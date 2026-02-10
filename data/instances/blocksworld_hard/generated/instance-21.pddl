

(define (problem BW-rand-12)
(:domain blocksworld-4ops)
(:objects a b c d e f g h i j k l )
(:init
(handempty)
(on a h)
(on b c)
(on c j)
(on d e)
(ontable e)
(on f i)
(on g l)
(on h k)
(ontable i)
(on j a)
(on k f)
(on l d)
(clear b)
(clear g)
)
(:goal
(and
(on a c)
(on b e)
(on c k)
(on d i)
(on f b)
(on g a)
(on h g)
(on i l)
(on j f)
(on k d)
(on l j))
)
)


