

(define (problem BW-rand-4)
(:domain blocksworld-4ops)
(:objects a b c d )
(:init
(handempty)
(ontable a)
(ontable b)
(ontable c)
(on d b)
(clear a)
(clear c)
(clear d)
)
(:goal
	(and
		(on a c)
		(on b a)
		(on d b)
		(on d a)
	)
)
)