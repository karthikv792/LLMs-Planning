

(define (problem BW-rand-10)
(:domain blocksworld-4ops)
(:objects a b c d e f g h i j )
(:init
(handempty)
(on a g)
(on b f)
(on c h)
(on d e)
(ontable e)
(ontable f)
(ontable g)
(on h b)
(ontable i)
(on j d)
(clear a)
(clear c)
(clear i)
(clear j)
)
(:goal
(and
(on a b)
(on b j)
(on c h)
(on d f)
(on e a)
(on g d)
(on h g)
(on j c))
)
)


