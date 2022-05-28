(define
	(problem grounded-BW-RAND-5)
	(:domain grounded-BLOCKSWORLD-4OPS)
	(:init
		(= (total-cost) 0)
		( CLEAR_D )
		( ON_E_B )
		( ON_D_A )
		( ONTABLE_C )
		( ON_B_C )
		( ON_A_E )
		( HANDEMPTY )
	)
	(:goal
		(and 
		( ON_D_E )
		)
	)
	(:metric minimize (total-cost))

)
