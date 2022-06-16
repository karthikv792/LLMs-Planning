(define
	(problem grounded-BW-RAND-4)
	(:domain grounded-BLOCKSWORLD-4OPS)
	(:init
		(= (total-cost) 0)
		( CLEAR_D )
		( CLEAR_A )
		( ON_D_C )
		( ONTABLE_C )
		( ONTABLE_B )
		( ON_A_B )
		( HANDEMPTY )
	)
	(:goal
		(and 
		( ON_C_A )
		)
	)
	(:metric minimize (total-cost))

)
