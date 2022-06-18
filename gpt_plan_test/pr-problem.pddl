(define
	(problem grounded-BW-RAND-4)
	(:domain grounded-BLOCKSWORLD-4OPS)
	(:init
		(= (total-cost) 0)
		( CLEAR_D )
		( CLEAR_A )
		( ON_D_B )
		( ONTABLE_C )
		( ON_B_C )
		( ONTABLE_A )
		( HANDEMPTY )
	)
	(:goal
		(and 
		( ON_D_A )
		( ON_C_B )
		)
	)
	(:metric minimize (total-cost))

)
