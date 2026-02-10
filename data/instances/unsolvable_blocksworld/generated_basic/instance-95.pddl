

(define (problem BW-rand-4)
(:domain blocksworld-4ops)
(:objects a b c d )
(:init
(handempty)
(on a d)
(on b a)
(on c b)
(ontable d)
(clear c)
)
(:goal
	(and
		(on b d)
		(on c a)
		(on d c)
		(on a c)
	)
)
)