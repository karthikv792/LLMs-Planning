

(define (problem BW-rand-9)
(:domain blocksworld-4ops)
(:objects a b c d e f g h i )
(:init
(handempty)
(ontable a)
(ontable b)
(on c h)
(on d c)
(on e a)
(on f g)
(on g i)
(ontable h)
(ontable i)
(clear b)
(clear d)
(clear e)
(clear f)
)
(:goal
(and
(on a f)
(on b e)
(on c h)
(on g a)
(on h b))
)
)


