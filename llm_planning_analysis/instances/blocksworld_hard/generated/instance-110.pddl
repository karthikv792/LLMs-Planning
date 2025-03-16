

(define (problem BW-rand-13)
(:domain blocksworld-4ops)
(:objects a b c d e f g h i j k l m )
(:init
(handempty)
(ontable a)
(on b i)
(on c b)
(on d l)
(on e k)
(on f a)
(on g f)
(on h e)
(on i g)
(on j m)
(on k d)
(on l c)
(on m h)
(clear j)
)
(:goal
(and
(on a i)
(on b e)
(on c a)
(on d b)
(on e h)
(on f m)
(on g j)
(on h k)
(on k l)
(on l g))
)
)


