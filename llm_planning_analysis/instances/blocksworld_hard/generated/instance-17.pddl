

(define (problem BW-rand-11)
(:domain blocksworld-4ops)
(:objects a b c d e f g h i j k )
(:init
(handempty)
(on a c)
(ontable b)
(on c h)
(on d e)
(ontable e)
(on f d)
(on g k)
(on h j)
(ontable i)
(on j i)
(on k a)
(clear b)
(clear f)
(clear g)
)
(:goal
(and
(on a d)
(on b j)
(on c a)
(on d k)
(on e f)
(on g b)
(on j e)
(on k g))
)
)


