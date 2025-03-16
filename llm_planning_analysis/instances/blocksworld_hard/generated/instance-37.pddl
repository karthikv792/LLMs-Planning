

(define (problem BW-rand-9)
(:domain blocksworld-4ops)
(:objects a b c d e f g h i )
(:init
(handempty)
(on a g)
(on b i)
(on c b)
(on d f)
(on e a)
(ontable f)
(on g h)
(on h c)
(ontable i)
(clear d)
(clear e)
)
(:goal
(and
(on a i)
(on b g)
(on c a)
(on g c)
(on h b)
(on i f))
)
)


