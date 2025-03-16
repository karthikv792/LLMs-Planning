

(define (problem BW-rand-12)
(:domain blocksworld-4ops)
(:objects a b c d e f g h i j k l )
(:init
(handempty)
(on a b)
(ontable b)
(ontable c)
(on d i)
(on e a)
(ontable f)
(on g k)
(on h d)
(on i l)
(on j h)
(on k j)
(ontable l)
(clear c)
(clear e)
(clear f)
(clear g)
)
(:goal
(and
(on a l)
(on b g)
(on c b)
(on d f)
(on e c)
(on f j)
(on g h)
(on i d)
(on j a))
)
)


