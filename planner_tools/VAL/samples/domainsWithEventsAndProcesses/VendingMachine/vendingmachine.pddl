(define (domain vending-machine)
(:requirements :fluents :continuous-effects :time)
(:predicates (slotopen) (coinfalling) (newcoin) (finished) (lightsensorstatus) (counterstatus))
(:functions (acc) (vel) (dist) (lightsensorposn) (trayposition) (counted))

(:action entercoin
:parameters ( )
:precondition (and (slotopen) (lightsensorstatus))
:effect (and (not (slotopen)) (coinfalling) (newcoin)
			(increase (acc) 1))
)

(:process falling
:parameters ( )
:precondition (coinfalling)
:effect (and (increase (dist) (* #t (vel)))
			(increase (vel) (* #t (acc))))
)

(:event lightsensoroff
:parameters ( )
:precondition (and (>= (dist) lightsensorposn) 		(lightsensorstatus) (newcoin)) 
:effect (and (not (lightsensorstatus)) 
		(counterstatus))
)

(:event lightsensoron
:parameters ( )
:precondition (and (not (lightsensorstatus)) (counterstatus) 
			(> (dist) lightsensorposn))
:effect (and (lightsensorstatus) (not (counterstatus)) 
			(not (newcoin)) (increase (counted) 1))
)

(:event received
:parameters ( )
:precondition (and (>= (dist) trayposition) (not (newcoin)) (coinfalling))
:effect (and (not (coinfalling)) (assign (dist) 0) 
		(assign (vel) 0) (assign (acc) 0) (slotopen))
)

(:action stop
:parameters ( )
:precondition (and (> (counted) 0) (slotopen))
:effect (finished)
)
)