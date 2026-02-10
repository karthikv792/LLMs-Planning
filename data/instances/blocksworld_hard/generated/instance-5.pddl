

(define (problem BW-rand-12)
(:domain blocksworld-4ops)
(:objects a b c d e f g h i j k l )
(:init
(handempty)
(ontable a)
(on b g)
(ontable c)
(on d e)
(on e l)
(on f d)
(on g a)
(on h f)
(on i k)
(ontable j)
(on k b)
(ontable l)
(clear c)
(clear h)
(clear i)
(clear j)
)
(:goal
(and
(on a l)
(on b d)
(on c b)
(on d g)
(on e f)
(on g j)
(on h c)
(on i k)
(on k a))
)
)


