

(define (problem BW-rand-9)
(:domain blocksworld-4ops)
(:objects a b c d e f g h i )
(:init
(handempty)
(on a f)
(on b d)
(on c g)
(on d i)
(on e a)
(on f c)
(ontable g)
(on h b)
(ontable i)
(clear e)
(clear h)
)
(:goal
(and
(on b f)
(on d b)
(on e c)
(on f h)
(on g a)
(on h i)
(on i g))
)
)


