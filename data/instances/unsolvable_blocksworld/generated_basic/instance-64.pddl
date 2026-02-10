

(define (problem BW-rand-5)
(:domain blocksworld-4ops)
(:objects a b c d e )
(:init
(handempty)
(on a b)
(on b d)
(ontable c)
(on d e)
(on e c)
(clear a)
)
(:goal
	(and
		(on a c)
		(on b a)
		(on c e)
		(on d e)
	)
)
)