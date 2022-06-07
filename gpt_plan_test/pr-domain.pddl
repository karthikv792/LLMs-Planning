(define
	(domain grounded-BLOCKSWORLD-4OPS)
	(:requirements :strips :action-costs)
	(:predicates
		( HOLDING_A )
		( CLEAR_B )
		( HOLDING_D )
		( CLEAR_C )
		( HOLDING_B )
		( HOLDING_C )
		( ONTABLE_A )
		( ONTABLE_D )
		( ON_A_A )
		( ON_A_C )
		( ON_A_D )
		( ON_B_A )
		( ON_B_B )
		( ON_B_C )
		( ON_B_D )
		( ON_C_A )
		( ON_C_B )
		( ON_C_C )
		( ON_C_D )
		( ON_D_A )
		( ON_D_B )
		( ON_D_D )
		( CLEAR_D )
		( HANDEMPTY )
		( CLEAR_A )
		( ONTABLE_C )
		( ONTABLE_B )
		( ON_D_C )
		( ON_A_B )
	) 
	(:functions (total-cost))
	(:action PICK-UP_D
		:parameters ()
		:precondition
		(and
			( HANDEMPTY )
			( ONTABLE_D )
			( CLEAR_D )
		)
		:effect
		(and
			(increase (total-cost) 1)
			( HOLDING_D )
			(not ( CLEAR_D ))
			(not ( ONTABLE_D ))
			(not ( HANDEMPTY ))
		)
	)
	(:action PICK-UP_A
		:parameters ()
		:precondition
		(and
			( HANDEMPTY )
			( ONTABLE_A )
			( CLEAR_A )
		)
		:effect
		(and
			(increase (total-cost) 1)
			( HOLDING_A )
			(not ( CLEAR_A ))
			(not ( ONTABLE_A ))
			(not ( HANDEMPTY ))
		)
	)
	(:action UNSTACK_D_D
		:parameters ()
		:precondition
		(and
			( HANDEMPTY )
			( CLEAR_D )
			( ON_D_D )
		)
		:effect
		(and
			(increase (total-cost) 1)
			( HOLDING_D )
			(not ( ON_D_D ))
			(not ( HANDEMPTY ))
		)
	)
	(:action UNSTACK_D_B
		:parameters ()
		:precondition
		(and
			( HANDEMPTY )
			( CLEAR_D )
			( ON_D_B )
		)
		:effect
		(and
			(increase (total-cost) 1)
			( HOLDING_D )
			( CLEAR_B )
			(not ( ON_D_B ))
			(not ( CLEAR_D ))
			(not ( HANDEMPTY ))
		)
	)
	(:action UNSTACK_D_A
		:parameters ()
		:precondition
		(and
			( HANDEMPTY )
			( CLEAR_D )
			( ON_D_A )
		)
		:effect
		(and
			(increase (total-cost) 1)
			( HOLDING_D )
			( CLEAR_A )
			(not ( ON_D_A ))
			(not ( CLEAR_D ))
			(not ( HANDEMPTY ))
		)
	)
	(:action UNSTACK_C_D
		:parameters ()
		:precondition
		(and
			( HANDEMPTY )
			( CLEAR_C )
			( ON_C_D )
		)
		:effect
		(and
			(increase (total-cost) 1)
			( HOLDING_C )
			( CLEAR_D )
			(not ( ON_C_D ))
			(not ( CLEAR_C ))
			(not ( HANDEMPTY ))
		)
	)
	(:action UNSTACK_C_C
		:parameters ()
		:precondition
		(and
			( HANDEMPTY )
			( CLEAR_C )
			( ON_C_C )
		)
		:effect
		(and
			(increase (total-cost) 1)
			( HOLDING_C )
			(not ( ON_C_C ))
			(not ( HANDEMPTY ))
		)
	)
	(:action UNSTACK_C_B
		:parameters ()
		:precondition
		(and
			( HANDEMPTY )
			( CLEAR_C )
			( ON_C_B )
		)
		:effect
		(and
			(increase (total-cost) 1)
			( HOLDING_C )
			( CLEAR_B )
			(not ( ON_C_B ))
			(not ( CLEAR_C ))
			(not ( HANDEMPTY ))
		)
	)
	(:action UNSTACK_C_A
		:parameters ()
		:precondition
		(and
			( HANDEMPTY )
			( CLEAR_C )
			( ON_C_A )
		)
		:effect
		(and
			(increase (total-cost) 1)
			( HOLDING_C )
			( CLEAR_A )
			(not ( ON_C_A ))
			(not ( CLEAR_C ))
			(not ( HANDEMPTY ))
		)
	)
	(:action UNSTACK_B_D
		:parameters ()
		:precondition
		(and
			( HANDEMPTY )
			( CLEAR_B )
			( ON_B_D )
		)
		:effect
		(and
			(increase (total-cost) 1)
			( HOLDING_B )
			( CLEAR_D )
			(not ( ON_B_D ))
			(not ( CLEAR_B ))
			(not ( HANDEMPTY ))
		)
	)
	(:action UNSTACK_B_C
		:parameters ()
		:precondition
		(and
			( HANDEMPTY )
			( CLEAR_B )
			( ON_B_C )
		)
		:effect
		(and
			(increase (total-cost) 1)
			( HOLDING_B )
			( CLEAR_C )
			(not ( ON_B_C ))
			(not ( CLEAR_B ))
			(not ( HANDEMPTY ))
		)
	)
	(:action UNSTACK_B_B
		:parameters ()
		:precondition
		(and
			( HANDEMPTY )
			( CLEAR_B )
			( ON_B_B )
		)
		:effect
		(and
			(increase (total-cost) 1)
			( HOLDING_B )
			(not ( ON_B_B ))
			(not ( HANDEMPTY ))
		)
	)
	(:action UNSTACK_B_A
		:parameters ()
		:precondition
		(and
			( HANDEMPTY )
			( CLEAR_B )
			( ON_B_A )
		)
		:effect
		(and
			(increase (total-cost) 1)
			( HOLDING_B )
			( CLEAR_A )
			(not ( ON_B_A ))
			(not ( CLEAR_B ))
			(not ( HANDEMPTY ))
		)
	)
	(:action UNSTACK_A_D
		:parameters ()
		:precondition
		(and
			( HANDEMPTY )
			( CLEAR_A )
			( ON_A_D )
		)
		:effect
		(and
			(increase (total-cost) 1)
			( HOLDING_A )
			( CLEAR_D )
			(not ( ON_A_D ))
			(not ( CLEAR_A ))
			(not ( HANDEMPTY ))
		)
	)
	(:action UNSTACK_A_C
		:parameters ()
		:precondition
		(and
			( HANDEMPTY )
			( CLEAR_A )
			( ON_A_C )
		)
		:effect
		(and
			(increase (total-cost) 1)
			( HOLDING_A )
			( CLEAR_C )
			(not ( ON_A_C ))
			(not ( CLEAR_A ))
			(not ( HANDEMPTY ))
		)
	)
	(:action UNSTACK_A_A
		:parameters ()
		:precondition
		(and
			( HANDEMPTY )
			( CLEAR_A )
			( ON_A_A )
		)
		:effect
		(and
			(increase (total-cost) 1)
			( HOLDING_A )
			(not ( ON_A_A ))
			(not ( HANDEMPTY ))
		)
	)
	(:action STACK_D_D
		:parameters ()
		:precondition
		(and
			( HOLDING_D )
			( CLEAR_D )
		)
		:effect
		(and
			(increase (total-cost) 1)
			( HANDEMPTY )
			( ON_D_D )
			(not ( HOLDING_D ))
		)
	)
	(:action STACK_D_C
		:parameters ()
		:precondition
		(and
			( HOLDING_D )
			( CLEAR_C )
		)
		:effect
		(and
			(increase (total-cost) 1)
			( HANDEMPTY )
			( CLEAR_D )
			( ON_D_C )
			(not ( CLEAR_C ))
			(not ( HOLDING_D ))
		)
	)
	(:action STACK_D_B
		:parameters ()
		:precondition
		(and
			( HOLDING_D )
			( CLEAR_B )
		)
		:effect
		(and
			(increase (total-cost) 1)
			( HANDEMPTY )
			( CLEAR_D )
			( ON_D_B )
			(not ( CLEAR_B ))
			(not ( HOLDING_D ))
		)
	)
	(:action STACK_D_A
		:parameters ()
		:precondition
		(and
			( HOLDING_D )
			( CLEAR_A )
		)
		:effect
		(and
			(increase (total-cost) 1)
			( HANDEMPTY )
			( CLEAR_D )
			( ON_D_A )
			(not ( CLEAR_A ))
			(not ( HOLDING_D ))
		)
	)
	(:action STACK_C_D
		:parameters ()
		:precondition
		(and
			( HOLDING_C )
			( CLEAR_D )
		)
		:effect
		(and
			(increase (total-cost) 1)
			( HANDEMPTY )
			( CLEAR_C )
			( ON_C_D )
			(not ( CLEAR_D ))
			(not ( HOLDING_C ))
		)
	)
	(:action STACK_C_C
		:parameters ()
		:precondition
		(and
			( HOLDING_C )
			( CLEAR_C )
		)
		:effect
		(and
			(increase (total-cost) 1)
			( HANDEMPTY )
			( ON_C_C )
			(not ( HOLDING_C ))
		)
	)
	(:action STACK_C_B
		:parameters ()
		:precondition
		(and
			( HOLDING_C )
			( CLEAR_B )
		)
		:effect
		(and
			(increase (total-cost) 1)
			( HANDEMPTY )
			( CLEAR_C )
			( ON_C_B )
			(not ( CLEAR_B ))
			(not ( HOLDING_C ))
		)
	)
	(:action STACK_C_A
		:parameters ()
		:precondition
		(and
			( HOLDING_C )
			( CLEAR_A )
		)
		:effect
		(and
			(increase (total-cost) 1)
			( HANDEMPTY )
			( CLEAR_C )
			( ON_C_A )
			(not ( CLEAR_A ))
			(not ( HOLDING_C ))
		)
	)
	(:action STACK_B_D
		:parameters ()
		:precondition
		(and
			( HOLDING_B )
			( CLEAR_D )
		)
		:effect
		(and
			(increase (total-cost) 1)
			( HANDEMPTY )
			( CLEAR_B )
			( ON_B_D )
			(not ( CLEAR_D ))
			(not ( HOLDING_B ))
		)
	)
	(:action STACK_B_C
		:parameters ()
		:precondition
		(and
			( HOLDING_B )
			( CLEAR_C )
		)
		:effect
		(and
			(increase (total-cost) 1)
			( HANDEMPTY )
			( CLEAR_B )
			( ON_B_C )
			(not ( CLEAR_C ))
			(not ( HOLDING_B ))
		)
	)
	(:action STACK_B_B
		:parameters ()
		:precondition
		(and
			( HOLDING_B )
			( CLEAR_B )
		)
		:effect
		(and
			(increase (total-cost) 1)
			( HANDEMPTY )
			( ON_B_B )
			(not ( HOLDING_B ))
		)
	)
	(:action STACK_B_A
		:parameters ()
		:precondition
		(and
			( HOLDING_B )
			( CLEAR_A )
		)
		:effect
		(and
			(increase (total-cost) 1)
			( HANDEMPTY )
			( CLEAR_B )
			( ON_B_A )
			(not ( CLEAR_A ))
			(not ( HOLDING_B ))
		)
	)
	(:action STACK_A_D
		:parameters ()
		:precondition
		(and
			( HOLDING_A )
			( CLEAR_D )
		)
		:effect
		(and
			(increase (total-cost) 1)
			( HANDEMPTY )
			( CLEAR_A )
			( ON_A_D )
			(not ( CLEAR_D ))
			(not ( HOLDING_A ))
		)
	)
	(:action STACK_A_C
		:parameters ()
		:precondition
		(and
			( HOLDING_A )
			( CLEAR_C )
		)
		:effect
		(and
			(increase (total-cost) 1)
			( HANDEMPTY )
			( CLEAR_A )
			( ON_A_C )
			(not ( CLEAR_C ))
			(not ( HOLDING_A ))
		)
	)
	(:action STACK_A_B
		:parameters ()
		:precondition
		(and
			( HOLDING_A )
			( CLEAR_B )
		)
		:effect
		(and
			(increase (total-cost) 1)
			( HANDEMPTY )
			( CLEAR_A )
			( ON_A_B )
			(not ( CLEAR_B ))
			(not ( HOLDING_A ))
		)
	)
	(:action STACK_A_A
		:parameters ()
		:precondition
		(and
			( HOLDING_A )
			( CLEAR_A )
		)
		:effect
		(and
			(increase (total-cost) 1)
			( HANDEMPTY )
			( ON_A_A )
			(not ( HOLDING_A ))
		)
	)
	(:action PUT-DOWN_D
		:parameters ()
		:precondition
		(and
			( HOLDING_D )
		)
		:effect
		(and
			(increase (total-cost) 1)
			( CLEAR_D )
			( HANDEMPTY )
			( ONTABLE_D )
			(not ( HOLDING_D ))
		)
	)
	(:action PUT-DOWN_C
		:parameters ()
		:precondition
		(and
			( HOLDING_C )
		)
		:effect
		(and
			(increase (total-cost) 1)
			( CLEAR_C )
			( HANDEMPTY )
			( ONTABLE_C )
			(not ( HOLDING_C ))
		)
	)
	(:action PUT-DOWN_B
		:parameters ()
		:precondition
		(and
			( HOLDING_B )
		)
		:effect
		(and
			(increase (total-cost) 1)
			( CLEAR_B )
			( HANDEMPTY )
			( ONTABLE_B )
			(not ( HOLDING_B ))
		)
	)
	(:action PUT-DOWN_A
		:parameters ()
		:precondition
		(and
			( HOLDING_A )
		)
		:effect
		(and
			(increase (total-cost) 1)
			( CLEAR_A )
			( HANDEMPTY )
			( ONTABLE_A )
			(not ( HOLDING_A ))
		)
	)
	(:action PICK-UP_C
		:parameters ()
		:precondition
		(and
			( HANDEMPTY )
			( ONTABLE_C )
			( CLEAR_C )
		)
		:effect
		(and
			(increase (total-cost) 1)
			( HOLDING_C )
			(not ( CLEAR_C ))
			(not ( ONTABLE_C ))
			(not ( HANDEMPTY ))
		)
	)
	(:action PICK-UP_B
		:parameters ()
		:precondition
		(and
			( HANDEMPTY )
			( ONTABLE_B )
			( CLEAR_B )
		)
		:effect
		(and
			(increase (total-cost) 1)
			( HOLDING_B )
			(not ( CLEAR_B ))
			(not ( ONTABLE_B ))
			(not ( HANDEMPTY ))
		)
	)
	(:action UNSTACK_D_C
		:parameters ()
		:precondition
		(and
			( HANDEMPTY )
			( CLEAR_D )
			( ON_D_C )
		)
		:effect
		(and
			(increase (total-cost) 1)
			( HOLDING_D )
			( CLEAR_C )
			(not ( ON_D_C ))
			(not ( CLEAR_D ))
			(not ( HANDEMPTY ))
		)
	)
	(:action UNSTACK_A_B
		:parameters ()
		:precondition
		(and
			( HANDEMPTY )
			( CLEAR_A )
			( ON_A_B )
		)
		:effect
		(and
			(increase (total-cost) 1)
			( HOLDING_A )
			( CLEAR_B )
			(not ( ON_A_B ))
			(not ( CLEAR_A ))
			(not ( HANDEMPTY ))
		)
	)

)
