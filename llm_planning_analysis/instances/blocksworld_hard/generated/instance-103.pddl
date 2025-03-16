

(define (problem BW-rand-11)
(:domain blocksworld-4ops)
(:objects a b c d e f g h i j k )
(:init
(handempty)
(on a h)
(ontable b)
(on c i)
(on d e)
(on e k)
(on f a)
(on g j)
(ontable h)
(on i g)
(on j f)
(on k c)
(clear b)
(clear d)
)
(:goal
(and
(on a f)
(on b a)
(on d e)
(on f g)
(on g k)
(on h j)
(on i h)
(on j c)
(on k d))
)
)


