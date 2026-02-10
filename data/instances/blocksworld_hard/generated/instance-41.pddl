

(define (problem BW-rand-10)
(:domain blocksworld-4ops)
(:objects a b c d e f g h i j )
(:init
(handempty)
(on a j)
(on b d)
(on c b)
(on d i)
(on e g)
(ontable f)
(on g f)
(on h e)
(on i a)
(on j h)
(clear c)
)
(:goal
(and
(on a j)
(on b d)
(on c b)
(on d a)
(on f i)
(on g f)
(on i c)
(on j h))
)
)


