(define (domain sleepingbeauty)
(:requirements :fluents :continuous-effects :negative-preconditions :time)
(:predicates (windowclosed) (windowopen) (magnetoperational) (freshair) (circuit) (alarmdisabled) (alarmenabled) (voltage) (ringing) (almostawake) (deeplyasleep) (awake))
(:functions (charge) (resistance) (ringtime) )

(:action openwindow
:parameters ( )
:precondition (and (windowclosed) (magnetoperational))
:effect (and (not (magnetoperational)) (not (windowclosed)) (windowopen) (freshair))
)

(:action closewindow
:parameters ( )
:precondition (and (windowopen) (not (magnetoperational)))
:effect (and (magnetoperational) (not (windowopen)) (windowclosed))
)

(:event makecircuit
:parameters ( )
:precondition (and (not (magnetoperational)) (not (circuit)))
:effect (and (circuit))
)

(:event breakcircuit
:parameters ( )
:precondition (and (magnetoperational) (circuit))
:effect (and (not (circuit)) (assign (charge) 0))
)

(:event alarmtriggered
:parameters ( )
:precondition (and (circuit) (alarmdisabled) (voltage))
:effect (and (alarmenabled) (not (alarmdisabled)) (ringing))
)

(:event alarmdisabled
:parameters ( )
:precondition (and (not (circuit)) (alarmenabled) (ringing))
:effect (and (not (alarmenabled)) (not (ringing)) (alarmdisabled) (assign (ringtime) 0))
)


(:event voltageavailable
:parameters ( )
:precondition (and (>= (charge) 5) (not (voltage)))
:effect (and (voltage))
)


(:process ring
:parameters ( )
:precondition (ringing)
:effect (and (increase (ringtime) (* #t 1)))
)

(:event rouseprincess
:parameters ( )
:precondition (and (ringing) (>= (ringtime) 0.001) (deeplyasleep))
:effect (and (almostawake) (not (deeplyasleep)))
)

(:action kiss
:parameters ( )
:precondition (almostawake) 
:effect (and (awake) (not (almostawake)))
)

(:process chargecapacitor
:parameters ( )
:precondition (and (circuit) (not (voltage)))
:effect (and (increase (charge) (* #t (/ 1 (resistance))))) 
   ;assuming capacitance is chosen to make this linear
   ;approximation reasonable.
)

)
