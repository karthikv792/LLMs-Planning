

(define (problem BW-rand-5)
(:domain blocksworld-4ops)
(:objects a b c d e )
(:init
(handempty)
(ontable a)
(on b d)
(on c b)
(on d a)
(ontable e)
(clear c)
(clear e)
)
(:goal
	(and
		(on a e)
		(on b d)
		(on c a)
		(on d c)
		(on c e)
	)
)
)