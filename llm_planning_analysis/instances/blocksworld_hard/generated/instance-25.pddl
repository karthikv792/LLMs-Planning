

(define (problem BW-rand-10)
(:domain blocksworld-4ops)
(:objects a b c d e f g h i j )
(:init
(handempty)
(on a d)
(on b i)
(on c b)
(on d g)
(on e h)
(ontable f)
(on g j)
(on h f)
(ontable i)
(ontable j)
(clear a)
(clear c)
(clear e)
)
(:goal
(and
(on a e)
(on c b)
(on d j)
(on f i)
(on g a)
(on h c)
(on i g)
(on j f))
)
)


