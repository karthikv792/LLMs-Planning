

(define (problem BW-rand-5)
(:domain blocksworld-4ops)
(:objects a b c d e )
(:init
(handempty)
(on a e)
(ontable b)
(ontable c)
(ontable d)
(on e b)
(clear a)
(clear c)
(clear d)
)
(:goal
	(and
		(on c d)
		(on d e)
		(on e b)
		(on b c)
	)
)
)