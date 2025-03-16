

(define (problem BW-rand-11)
(:domain blocksworld-4ops)
(:objects a b c d e f g h i j k )
(:init
(handempty)
(ontable a)
(ontable b)
(on c i)
(on d b)
(on e k)
(on f h)
(ontable g)
(on h a)
(ontable i)
(on j c)
(on k d)
(clear e)
(clear f)
(clear g)
(clear j)
)
(:goal
(and
(on a h)
(on b j)
(on d e)
(on e k)
(on f g)
(on g d)
(on h b)
(on i c)
(on k a))
)
)


