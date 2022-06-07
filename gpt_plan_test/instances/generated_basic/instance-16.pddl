

(define (problem BW-rand-12)
(:domain blocksworld-4ops)
(:objects a b c d e f g h i j k l )
(:init
(handempty)
(ontable a)
(on b c)
(on c l)
(on d f)
(on e k)
(ontable f)
(on g b)
(on h d)
(on i a)
(on j h)
(on k g)
(on l i)
(clear e)
(clear j)
)
(:goal
(and
(on a c)
(on b d)
(on d i)
(on f k)
(on g j)
(on i l)
(on j e)
(on k g)
(on l h))
)
)


