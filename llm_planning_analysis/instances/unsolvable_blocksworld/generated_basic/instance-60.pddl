

(define (problem BW-rand-5)
(:domain blocksworld-4ops)
(:objects a b c d e )
(:init
(handempty)
(on a d)
(on b e)
(ontable c)
(ontable d)
(ontable e)
(clear a)
(clear b)
(clear c)
)
(:goal
	(and
		(on d a)
		(on e d)
		(on e a)
	)
)
)