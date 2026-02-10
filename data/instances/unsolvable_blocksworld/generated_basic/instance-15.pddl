

(define (problem BW-rand-5)
(:domain blocksworld-4ops)
(:objects a b c d e )
(:init
(handempty)
(on a c)
(on b a)
(on c e)
(ontable d)
(on e d)
(clear b)
)
(:goal
	(and
		(on b c)
		(on d e)
		(on e b)
		(on c d)
	)
)
)