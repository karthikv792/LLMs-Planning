

(define (problem BW-rand-13)
(:domain blocksworld-4ops)
(:objects a b c d e f g h i j k l m )
(:init
(handempty)
(on a b)
(on b f)
(on c i)
(on d j)
(ontable e)
(on f k)
(on g a)
(on h m)
(on i e)
(on j g)
(on k h)
(on l c)
(ontable m)
(clear d)
(clear l)
)
(:goal
(and
(on b e)
(on d l)
(on f a)
(on h b)
(on i j)
(on j h)
(on l c)
(on m i))
)
)


