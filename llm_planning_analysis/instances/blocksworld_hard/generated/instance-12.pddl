

(define (problem BW-rand-12)
(:domain blocksworld-4ops)
(:objects a b c d e f g h i j k l )
(:init
(handempty)
(on a d)
(ontable b)
(on c e)
(on d c)
(on e i)
(on f b)
(ontable g)
(ontable h)
(on i h)
(ontable j)
(on k l)
(on l j)
(clear a)
(clear f)
(clear g)
(clear k)
)
(:goal
(and
(on b e)
(on e g)
(on f b)
(on g a)
(on i d)
(on k f))
)
)


