

(define (problem BW-rand-5)
(:domain blocksworld-4ops)
(:objects a b c d e )
(:init
(handempty)
(on a b)
(on b c)
(on c d)
(on d e)
(ontable e)
(clear a)
)
(:goal
	(and
		(on a d)
		(on b c)
		(on e a)
		(on e c)
	)
)
)