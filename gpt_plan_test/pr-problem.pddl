(define
	(problem grounded-BW-RAND-4)
	(:domain grounded-BLOCKSWORLD-4OPS)
	(:init
		(= (total-cost) 0)
		( CLEAR_D )
		( CLEAR_B )
		( ON_D_C )
		( ON_C_A )
		( ONTABLE_B )
		( ONTABLE_A )
		( HANDEMPTY )
	)
	(:goal
		(and 
		( ON_D_B )
		( ON_C_D )
		)
	)
	(:metric minimize (total-cost))

)
