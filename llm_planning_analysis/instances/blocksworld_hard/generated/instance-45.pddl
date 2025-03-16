

(define (problem BW-rand-10)
(:domain blocksworld-4ops)
(:objects a b c d e f g h i j )
(:init
(handempty)
(ontable a)
(on b i)
(ontable c)
(on d a)
(on e c)
(on f h)
(on g d)
(ontable h)
(on i g)
(on j f)
(clear b)
(clear e)
(clear j)
)
(:goal
(and
(on c f)
(on d h)
(on f e)
(on g b)
(on h i)
(on i c)
(on j g))
)
)


