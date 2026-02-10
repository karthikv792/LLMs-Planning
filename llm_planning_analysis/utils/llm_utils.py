from transformers import StoppingCriteriaList, StoppingCriteria
from openai import OpenAI
import os
from transformers import StoppingCriteriaList, StoppingCriteria
import os
import time
import anthropic
from groq import Groq
import vertexai
from vertexai.language_models import TextGenerationModel
from google import genai
from google.genai import types 
from google.oauth2 import service_account
import boto3
import json
from botocore.config import Config
from rich import print

config = Config(read_timeout=1000)

aws_client = boto3.client('bedrock-runtime', region_name='us-west-2', config=config)
client = OpenAI()
groq_client = Groq()
def generate_from_bloom(model, tokenizer, query, max_tokens):
    encoded_input = tokenizer(query, return_tensors='pt')
    stop = tokenizer("[PLAN END]", return_tensors='pt')
    stoplist = StoppingCriteriaList([stop])
    output_sequences = model.generate(input_ids=encoded_input['input_ids'].cuda(), max_new_tokens=max_tokens,
                                      temperature=0, top_p=1)
    return tokenizer.decode(output_sequences[0], skip_special_tokes=True)

def connect_google(client, engine, messages, temperature, max_tokens,
                   frequency_penalty=None, presence_penalty=None):
    response = client.generate_content(messages,
                                       generation_config=genai.types.GenerationConfig(
                                           candidate_count=1,
                                           max_output_tokens=max_tokens,
                                           temperature=temperature),
                                       stream=False)
    # response.resolve()
    return response.text

def send_query(query, engine, max_tokens, model=None, stop="[STATEMENT]", params = {'temperature': 0.0, 'n': 1}):
    max_token_err_flag = False
    if engine == 'bloom':

        if model:
            response = generate_from_bloom(model['model'], model['tokenizer'], params, query, max_tokens)
            response = response.replace(query, '')
            resp_string = ""
            for line in response.split('\n'):
                if '[PLAN END]' in line:
                    break
                else:
                    resp_string += f'{line}\n'
            return resp_string
        else:
            assert model is not None
    elif engine == 'palm':
        # Change this to your own path or set the environment variable
        # os.environ["GOOGLE_APPLICATION_CREDENTIALS"] = "/home/local/ASUAD/kvalmeek/google-cloud-keys/llm-planning-715517cd41ec.json"
        vertexai.init(project='llm-planning')

        parameters = {
            'temperature': params['temperature']
        }
        
        model = TextGenerationModel.from_pretrained("text-bison@001")
        response  = model.predict(query, **parameters)
        return response.text.strip()
    elif 'gemini' in engine:
        GOOGLE_API_KEY = os.environ["GOOGLE_API_KEY"]
        genai.configure(api_key=GOOGLE_API_KEY)
        if 'gemini-2.5-thinking' in engine:
            engine = 'gemini-2.5-pro-exp-03-25'
        gemini_pro = genai.GenerativeModel(engine)
        response = connect_google(gemini_pro, engine, query, params['temperature'], max_tokens)
        time.sleep(45)
        return response.strip()
        # vertexai.init(project="llm-planning")
        # parameters={
        #     'temperature': params['temperature']
        # }
        # model = GenerativeModel(engine)
        # gen_config = GenerationConfig(temperature=1.0)
        # response = model.generate_content(query, generation_config=gen_config)
        # return response.text.strip()
    elif engine == 'claude-3-opus':
        response = anthropic.Anthropic().messages.create(
            model="claude-3-opus-20240229",
            max_tokens=max_tokens,
            temperature=params['temperature'],
            system = "You are the planner assistant who comes up with correct plans.",
            messages = [{"role": "user", "content": query}]
        )
        try:
            for i in response.content:
                if i.type == 'text':
                    return i.text.strip()
        except Exception as e:
            print("[-]: Failed GPT3 query execution: {}".format(e))
            print(response)
    elif engine == 'claude-3.5-sonnet':
        response = anthropic.Anthropic().messages.create(
            model="claude-3-5-sonnet-20240620",
            max_tokens=max_tokens,
            temperature=params['temperature'],
            system = "You are the planner assistant who comes up with correct plans.",
            messages = [{"role": "user", "content": query}]
        )
        try:
            for i in response.content:
                if i.type == 'text':
                    return i.text.strip()
        except Exception as e:
            print("[-]: Failed GPT3 query execution: {}".format(e))
            print(response)

    elif engine == 'finetuned':
        if model:
            try:
                response = openai.Completion.create(
                    model=model['model'],
                    prompt=query,
                    temperature=params['temperature'],
                    max_tokens=max_tokens,
                    top_p=1,
                    frequency_penalty=0,
                    presence_penalty=0,
                    stop=["[PLAN END]"])
            except Exception as e:
                max_token_err_flag = True
                print("[-]: Failed GPT3 query execution: {}".format(e))
            text_response = response["choices"][0]["text"] if not max_token_err_flag else ""
            return text_response.strip()
        else:
            assert model is not None
    elif '_chat' in engine:
        # gpt-4-turbo-2024-04-09
        eng = engine.split('_')[0]
        # print('chatmodels', eng)
        messages=[
        # {"role": "system", "content": "You are a planner assistant who comes up with correct plans."},
        {"role": "user", "content": query}
        ]
        try:
            s_time = time.time()
            response = client.chat.completions.create(model=eng, messages=messages)#, temperature=params['temperature'])
            e_time = time.time()
            time_taken = e_time - s_time
        except Exception as e:
            max_token_err_flag = True
            print("[-]: Failed GPT3 query execution: {}".format(e))
            time.sleep(3000)
        text_response = response.choices[0].message.content if not max_token_err_flag else "" 
        # print(response)
        print(response.usage)
        # print(response.usage.completion_tokens_details["reasoning_tokens"])
        return text_response.strip(), response, time_taken
    elif '_groq' in engine:
        eng = engine.split('_')[0]
        messages=[
        {"role": "system", "content": "You are a planner assistant who comes up with correct plans."},
        {"role": "user", "content": query}
        ]
        temp = 0.0001 if params['temperature'] == 0.0 else params['temperature']
        try:
            response = groq_client.chat.completions.create(messages=messages, model=eng, temperature=temp)
        except Exception as e:
            max_token_err_flag = True
            print("[-]: Failed GPT3 query execution: {}".format(e))
        text_response = response.choices[0].message.content if not max_token_err_flag else ""
        time.sleep(2)
        return text_response.strip()
    elif '_aws' in engine:
        flag='claude'
        if engine.split('_')[0] == 'claude-3-sonnet':
            model_id = "anthropic.claude-3-sonnet-20240229-v1:0"
        elif engine.split('_')[0] == 'claude-3-opus':
            model_id = "anthropic.claude-3-opus-20240229-v1:0"
        elif engine.split('_')[0] == 'claude-3.5-sonnet':
            model_id = "anthropic.claude-3-5-sonnet-20240620-v1:0"
        elif engine.split('_')[0] == 'llama-3.1-405b':
            model_id = "meta.llama3-1-405b-instruct-v1:0"
            flag='meta'
        else:
            raise Exception("[-]: Invalid engine name: {}".format(engine))
        if flag=='claude':
            try:
                response = aws_client.invoke_model(
                    modelId=model_id,
                    body=json.dumps(
                        {
                            "anthropic_version": "bedrock-2023-05-31",
                            "max_tokens": 5012,
                            "messages": [
                                {
                                    "role": "user",
                                    "content": [{"type": "text", "text": query}],
                                }
                            ],
                            "temperature": params['temperature'],
                        }
                    ),
                )

                # Process and print the response
                result = json.loads(response.get("body").read())
                output_list = result.get("content", [])
                if output_list:
                    return output_list[0]["text"].strip()
            except Exception as e:
                print("[-]: Failed CLAUDE query execution: {}".format(e))
        elif flag=='meta':
            query_to_send = f"<|begin_of_text|><|start_header_id|>system<|end_header_id|>\n You are the planner assistant who comes up with correct plans.<|eot_id|>\n<|start_header_id|>user<|end_header_id|>\n{query}<|eot_id|><|start_header_id|>assistant<|end_header_id|>"
            try:
                st = time.time()
                response = aws_client.invoke_model(
                    modelId=model_id,
                    body=json.dumps(
                        {
                            "max_gen_len": 2048,
                            "prompt": query_to_send,
                            "temperature": params['temperature'],
                        }
                    ),
                )
                et = time.time()

                # Process and print the response
                result = json.loads(response.get("body").read())
                output = result["generation"]
                return output.strip(), result, et-st
            except Exception as e:
                print("[-]: Failed LLAMA query execution: {}".format(e))

        
    else:
        try:
            response = client.completions.create(
                model=engine,
                prompt=query,
                temperature=params['temperature'],
                top_p=1,
                frequency_penalty=0,
                presence_penalty=0,
                stop=stop)
        except Exception as e:
            max_token_err_flag = True
            print("[-]: Failed GPT3 query execution: {}".format(e))

        text_response = response.choices[0].text if not max_token_err_flag else ""
        return text_response.strip()


def send_query_multiple_for_loop(query, engine, max_tokens, params, model=None, stop="[STATEMENT]"):
    max_token_err_flag = False
    if '_chat' in engine:
        eng = engine.split('_')[0]
        

def send_query_multiple(query, engine, max_tokens, params, model=None, stop="[STATEMENT]"):
    max_token_err_flag = False
    if engine == 'finetuned':
        if model:
            #STILL using n within API
            try:
                response = client.chat.completions.create(
                    model=model['model'],
                    prompt=query,
                    temperature=params['temperature'],
                    n = params['n'],
                    max_tokens=max_tokens,
                    top_p=1,
                    frequency_penalty=0,
                    presence_penalty=0,
                    stop=["[PLAN END]"])
            except Exception as e:
                max_token_err_flag = True
                print("[-]: Failed GPT3 query execution: {}".format(e))
            text_responses = dict([(ind,resp["text"].strip()) for ind, resp in enumerate(response["choices"])]) if not max_token_err_flag else ""
            
            # text_response = response["choices"][0]["text"] if not max_token_err_flag else ""
            return text_responses
        else:
            assert model is not None
    elif '_chat' in engine:
        
        eng = engine.split('_')[0]
        # print('chatmodels', eng)
        text_responses = {}
        total_responses = 0
        while total_responses < params['n']:
            messages=[
            {"role": "system", "content": "You are the planner assistant who comes up with correct plans."},
            {"role": "user", "content": query}
            ]
            try:
                response = client.chat.completions.create(model=eng, messages=messages, temperature=params['temperature'])
                text_responses[total_responses] = response.choices[0].message.content
            except Exception as e:
                if 'Request timed out' in str(e):
                    time.sleep(1)
                    continue
                max_token_err_flag = True
                print("[-]: Failed GPT3 query execution: {}".format(e))
            time.sleep(0.5)
            total_responses += 1
        # text_responses = dict([(ind,resp["message"]["content"].strip()) for ind, resp in enumerate(response["choices"])]) if not max_token_err_flag else ""
        if not max_token_err_flag:
            return text_responses
        else:
            return ""
        # text_response = response['choices'][0]['message']['content'] if not max_token_err_flag else ""
        # return text_responses
    else:
        try:
            response = client.chat.completions.create(
                model=engine,
                prompt=query,
                temperature=params['temperature'],
                max_tokens=max_tokens,
                top_p=1,
                n=params['n'],
                frequency_penalty=0,
                presence_penalty=0,
                stop=stop)
        except Exception as e:
            max_token_err_flag = True
            print("[-]: Failed GPT3 query execution: {}".format(e))

        text_responses = dict([(ind,resp["text"].strip()) for ind, resp in enumerate(response["choices"])]) if not max_token_err_flag else ""
        return text_responses
    


def send_query_with_feedback(query, engine, messages=[], history=-1, temp=0):
    err_flag = False
    context_window_hit = False
    rate_limit_hit = False
    null_response = False
    
    if len(messages) == 0:
        # Add if for all models
        if "o1-preview" in engine or "o1-mini" in engine or "deepseek-r1" in engine or "gemini-2.5-thinking" in engine:
            messages=[
        # {"role": "system", "content": "You are the planner assistant who comes up with correct plans."},
        {"role": "user", "content": query}
        ]
        else:
            messages=[
        {"role": "system", "content": "You are the planner assistant who comes up with correct plans."},
        {"role": "user", "content": query}
        ]
    else:
        #Just for validation message - query consists of the validation message
        messages.append({"role": "user", "content": query})
    if history==-1:
        sending_messages = messages
        # temp = 1
    else:
        if "o1-preview" in engine or "o1-mini" in engine or "deepseek-r1" in engine or "gemini-2.5-thinking" in engine:
            if len(messages) == 1:
                sending_messages = messages[:1]
            else:
                sending_messages = messages[:1]+messages[-history*2:] if history>0 else messages[:1]
            
        else:
            if len(messages) == 2:
                sending_messages = messages[:2]
            else:
                sending_messages = messages[:2]+messages[-history*2:] if history>0 else messages[:2]
        # print('sending_messages', sending_messages)
        # temp = 0
    # print('sending_messages', sending_messages)
    if '_chat' in engine:
        eng = engine.split('_')[0]
        # print('chatmodels', eng)
        
        try:
            if "o1-" in eng:
                st = time.time()
                response = client.chat.completions.create(model=eng, messages=sending_messages)
                et = time.time()
            else:
                st = time.time()
                response = client.chat.completions.create(model=eng, messages=sending_messages, temperature=temp)
                et = time.time()
        except Exception as e: 
            err_flag = True
            if "maximum context length" in str(e):
                context_window_hit = True
            print("[-]: Failed GPT3 query execution: {}".format(e))
            st, et = 0, 0
        text_response = "" if err_flag else response.choices[0].message.content
        if not text_response or text_response.isspace():
            null_response = True
            text_response = ""
        messages.append({"role": "assistant", "content": text_response})
        return text_response.strip(), messages, context_window_hit, rate_limit_hit, response, et-st, sending_messages, null_response
    elif 'deepseek-r1' in engine:
        deepseek_api = os.environ["DEEPSEEK_API_KEY"]
        deepseek_client = OpenAI(api_key=deepseek_api, base_url="https://api.deepseek.com")
        try:
            st = time.time()
            response = deepseek_client.chat.completions.create(model="deepseek-reasoner", messages=sending_messages, stream=False)
            et = time.time()
        except Exception as e:
            err_flag = True
            if "maximum context length" in str(e):
                context_window_hit = True
            print("[-]: Failed DeepSeek query execution: {}".format(e))
            st, et = 0, 0 
            response = None
        text_response = "" if err_flag else response.choices[0].message.content
        if not text_response or text_response.isspace():
            null_response = True
            text_response = ""
        messages.append({"role": "assistant", "content": text_response})
        return text_response.strip(), messages, context_window_hit, rate_limit_hit, response, et-st, sending_messages, null_response
    elif 'gemini-2.5-thinking' in engine:
        engine = 'gemini-2.5-pro-preview-03-25'#'gemini-2.5-pro-exp-03-25'
        gemini_api = os.environ["GOOGLE_API_KEY"]
        client = genai.Client(api_key=gemini_api, http_options={'api_version':'v1alpha'})
        """
        chat = client.chats.create(
        model="gemini-2.0-flash",
        history=[
            types.Content(role="user", parts=[types.Part(text="Hello")]),
            types.Content(
                role="model",
                parts=[
                    types.Part(
                        text="Great to meet you. What would you like to know?"
                    )
                ],
            ),
        ],
    )
        """
        history = []
        for message in sending_messages[:-1]:
            history.append(types.Content(role=message['role'] if message['role'] == 'user' else 'model', parts=[types.Part(text=message['content'])]))
        chat = client.chats.create(model=engine, history=history)
        config_gemini = {'thinking_config': {'include_thoughts': True}}
        s_time = time.time()
        try:
            response = chat.send_message(sending_messages[-1]['content'])
            e_time = time.time()
        except Exception as e:
            err_flag = True
            print("[-]: Failed Gemini query execution: {}".format(e))
            s_time, e_time = 0, 0
            response = None
            
        gemini_response = {
            "thought": "",
            "response": ""
        }
        if response == None:
            null_response = True
            return "", messages, False, False, response, e_time-s_time, sending_messages, null_response
            
        for part in response.candidates[0].content.parts:
            if part.thought:
                gemini_response["thought"] += part.text
            else:
                gemini_response["response"] += part.text
        print(gemini_response)
        time.sleep(70)
        if not gemini_response["response"] or gemini_response["response"].isspace():
            null_response = True
            gemini_response["response"] = ""
        return gemini_response["response"].strip(), messages, False, False, response, e_time-s_time, sending_messages, null_response
    elif '_aws' in engine:
        if engine.split('_')[0] == 'llama-3.1-405b':
            model_id = "meta.llama3-1-405b-instruct-v1:0"
            flag='meta'
        else:
            raise("[-]: Invalid engine name: {}".format(engine))
        query_to_send = llama_messages_to_single_prompt(messages)
        try:
            st = time.time()
            response = aws_client.invoke_model(
                modelId=model_id,
                body=json.dumps(
                    {
                        "max_gen_len": 2048,
                        "prompt": query_to_send,
                        "temperature": temp,
                    }
                ),
            )
            et = time.time()

            # Process and print the response
            result = json.loads(response.get("body").read())
            output = result["generation"]
            if not output or output.isspace():
                null_response = True
                output = ""
            messages.append({"role": "assistant", "content": output.strip()})
            return output.strip(), messages, False, False, result, et-st, sending_messages, null_response
        except Exception as e:
            print("[-]: Failed LLAMA query execution: {}".format(e))
            return "", messages, False, False, None, 0, sending_messages, True
        
    else:
        raise Exception("[-]: Invalid engine name: {}".format(engine))

def llama_messages_to_single_prompt(messages):
    #f"<|begin_of_text|><|start_header_id|>system<|end_header_id|>\n You are the planner assistant who comes up with correct plans.<|eot_id|>\n<|start_header_id|>user<|end_header_id|>\n{query}<|eot_id|><|start_header_id|>assistant<|end_header_id|>"
    prompt = "<|begin_of_text|>"
    for message in messages:
        role = message['role']
        content = message['content']
        prompt += f"<|start_header_id|>{role}<|end_header_id|>\n{content}<|eot_id|>\n"
    prompt += "<|start_header_id|>assistant<|end_header_id|>"
    return prompt


# def save_gpt3_response(planexecutor, response, file):
#     action_list = list(planexecutor.model["domain"].keys())
#     action_list = [act.lower() for act in action_list]
#     plan = []
#     for line in response.split('\n'):
#         if '[PLAN END]' in line:
#             break
#         else:
#             action = line[line.find("(")+1:line.find(")")]
#             if not action.strip():
#                 continue
#             act_name = action.strip().split()[0]    
#             if act_name and act_name.lower() in action_list:
#             #find elements between ()
#                 plan.append(f'({action})')
#     with open(file, 'w') as f:
#         f.write('\n'.join(plan))
#     return '\n'.join(plan)
def save_gpt3_response(response, file):
    # action_list = list(planexecutor.model["domain"].keys())
    # action_list = [act.lower() for act in action_list]
    # plan = []
    plan = []
    for line in response.split('\n'):
        if not line.startswith('('):
            continue
        if '()' in line:
            continue
        if 'no actions' in line:
            continue
        plan.append(line.lower().strip())
    #     if '[PLAN END]' in line:
    #         break
    #     else:
    #         # action = line[line.find("(")+1:line.find(")")]
    #         # if not action.strip():
    #         #     continue
    #         # act_name = action.strip().split()[0]    
    #         # if act_name and act_name.lower() in action_list:
    #         # #find elements between ()
    #         #     plan.append(f'({action})')
    with open(file, 'w') as f:
        f.write('\n'.join(plan))
    return '\n'.join(plan)

    

    