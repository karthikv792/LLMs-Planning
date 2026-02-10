

(define (problem BW-rand-11)
(:domain blocksworld-4ops)
(:objects a b c d e f g h i j k )
(:init
(handempty)
(on a c)
(ontable b)
(ontable c)
(on d e)
(on e k)
(on f a)
(on g h)
(ontable h)
(on i b)
(on j f)
(ontable k)
(clear d)
(clear g)
(clear i)
(clear j)
)
(:goal
(and
(on b j)
(on c e)
(on d b)
(on e k)
(on f g)
(on g h)
(on h d)
(on i a)
(on j c)
(on k i))
)
)


