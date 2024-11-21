(define (domain sleepingbeauty2)
(:requirements :fluents :continuous-effects :negative-preconditions :time)
(:predicates (windowclosed) (windowopen) (magnetoperational) (freshair) (circuit) (alarmdisabled) (alarmenabled) (voltage) (ringing) (almostawake) (deeplyasleep) (awake) )
(:functions (ringtime) )

(:action openwindow
:parameters ( )
:precondition (and (windowclosed) (magnetoperational))
:effect (and (not (magnetoperational)) (not (windowclosed)) (windowopen) (freshair))
	 ;; Having fresh air is a possible goal which forces
	;; the window to be opened. 
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

(:event alarmtriggered
:parameters ( )
:precondition (and (circuit) (alarmdisabled) )
:effect (and (not (alarmdisabled)) (ringing) )
)

(:event breakcircuit
:parameters ( )
:precondition (and (magnetoperational) (circuit))
:effect (and (not (circuit)))
)


(:event alarmterminated
:parameters ( )
:precondition (and (ringing) (not (circuit)))
:effect (and (not (ringing)) (alarmdisabled))
)


(:process ring
:parameters ( )
:precondition (ringing)
:effect (and (increase (ringtime) (* #t 1)))
)

(:event rouseprincess
:parameters ( )
:precondition (and (ringing) (>= (ringtime) 0.001) 
	;; If this time delay is longer then it is possible to
	;; open the window and close it before Sleeping Beauty 
	;; wakes up, but otherwise giving her fresh air will  
	;; rouse her (because of the alarm).
	(deeplyasleep))
:effect (and (almostawake) (not (deeplyasleep)))
)

(:action kiss
:parameters ( )
:precondition (almostawake) 
:effect (and (awake) (not (almostawake)))
)

)
