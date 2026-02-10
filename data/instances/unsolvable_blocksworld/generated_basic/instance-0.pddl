

(define (problem BW-rand-5)
(:domain blocksworld-4ops)
(:objects a b c d e )
(:init
(handempty)
(on a e)
(ontable b)
(on c b)
(ontable d)
(on e d)
(clear a)
(clear c)
)
(:goal
	(and
		(on a b)
		(on b c)
		(on c e)
		(on d a)
		(on c b)
	)
)
)