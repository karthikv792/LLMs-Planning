

(define (problem BW-rand-5)
(:domain blocksworld-4ops)
(:objects a b c d e )
(:init
(handempty)
(on a e)
(ontable b)
(ontable c)
(ontable d)
(ontable e)
(clear a)
(clear b)
(clear c)
(clear d)
)
(:goal
	(and
		(on a c)
		(on d e)
		(on e c)
	)
)
)