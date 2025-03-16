

(define (problem BW-rand-9)
(:domain blocksworld-4ops)
(:objects a b c d e f g h i )
(:init
(handempty)
(on a c)
(on b a)
(on c g)
(ontable d)
(on e d)
(on f e)
(on g f)
(on h b)
(on i h)
(clear i)
)
(:goal
(and
(on a g)
(on d c)
(on e f)
(on f d)
(on g i)
(on i b))
)
)


