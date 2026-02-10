

(define (problem BW-rand-11)
(:domain blocksworld-4ops)
(:objects a b c d e f g h i j k )
(:init
(handempty)
(on a j)
(ontable b)
(on c i)
(on d g)
(on e k)
(on f h)
(ontable g)
(ontable h)
(on i a)
(on j d)
(on k b)
(clear c)
(clear e)
(clear f)
)
(:goal
(and
(on b j)
(on c a)
(on d h)
(on f e)
(on g d)
(on i k))
)
)


