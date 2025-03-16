

(define (problem BW-rand-5)
(:domain blocksworld-4ops)
(:objects a b c d e )
(:init
(handempty)
(ontable a)
(on b d)
(ontable c)
(ontable d)
(on e a)
(clear b)
(clear c)
(clear e)
)
(:goal
	(and
		(on c e)
		(on d c)
		(on e c)
	)
)
)