

(define (problem BW-rand-5)
(:domain blocksworld-4ops)
(:objects a b c d e )
(:init
(handempty)
(on a d)
(on b a)
(on c e)
(on d c)
(ontable e)
(clear b)
)
(:goal
	(and
		(on b d)
		(on c e)
		(on d a)
		(on a d)
	)
)
)