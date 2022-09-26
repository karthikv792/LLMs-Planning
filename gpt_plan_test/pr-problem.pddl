(define
	(problem grounded-BW-RAND-4)
	(:domain grounded-BLOCKSWORLD-4OPS)
	(:init
		(= (total-cost) 0)
		( CLEAR_C )
		( ONTABLE_D )
		( ON_C_A )
		( ON_B_D )
		( ON_A_B )
		( HANDEMPTY )
	)
	(:goal
		(and 
		( ON_D_C )
		( ON_B_D )
		)
	)
	(:metric minimize (total-cost))

)
