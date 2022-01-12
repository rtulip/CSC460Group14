# Introduction

This is a project that [myself](https://github.com/rtulip) and a partner [Ben Hillier](https://github.com/BenDHillier) made for [CSC 460](https://heat.csc.uvic.ca/coview/course/2019011/CSC460) at UVic. The course was an exploration in real-time systems, and our project was to create a non-standard [RTOS](https://en.wikipedia.org/wiki/Real-time_operating_system) for an Arduino Mega 2560. This repository is our work back in 2019, but has been since been cleaned up to make it a bit easier to understand.

## RTOS Requirements

Unfortunately, the course website is no longer available, so I can't provide the exact requirements, but from memory, we were challenged to create a RTOS with the following requirements:
1. The RTOS must be non-premptive.
    * This was the major requirement that made this RTOS design non-standard. A standard RTOS will interrupt tasks of lower priority to make sure that high priority periodic tasks always run on time. Instead, our RTOS had to run each task and throw errors if tasks ran late or were delayed. The benefit of such an approach is that issues such as [Priority Inversion](https://en.wikipedia.org/wiki/Priority_inversion) and [Race Conditions](https://en.wikipedia.org/wiki/Race_condition) because each task is run to completion.
2. The RTOS must be able to service periodic tasks
    * Tasks must be able to have different priorities
    * Tasks must be stateful.
3. The RTOS must be able to support non-periodic tasks (events).
    * Events must only be run once
    * Events must not disrupt periodic tasks.
4. The RTOS must be able to detect and handle Errors
    * Errors must be able to have different levels of severity
    * Custom error handlers must be supported.

## Design

This RTOS works on a modified [Time-Triggered Architecture](https://en.wikipedia.org/wiki/Time-triggered_architecture) (TTA) where tasks are dispatched when each task's timer counts down to zero. If multiple tasks would be triggered at the same time (due to some task running late), the task with the highest priority is dispatched.

Tasks are initialized with the period they're supposed to repeat by and a phase/delay. For instance, in `rtos_demo/main.c`, the tasks `Ping` and `Pong` both are supposed to run every 50 ms, and `Pong` is offset by 10ms to ensure that it always runs after `Ping`. If a task run's and delays another task, then errors get sent to the error handler. Users can create an error handling function to resolve any issues caused by the previous task running late. The error handler is called before the periodic task is run. Currently, the error severity reflects if a task is slightly delayed (<2ms), slightly delayed repeatedly (<2ms 3 or more times), or if a deadline was missed (based off of user defined tolerances).

The non-standard part of a TTA is the support of events. Tasks may need to do certain jobs in specific conditions, but don't need to be done repeatedly, thus the task would create an event. Furthermore tasks may need to assert that some condition has been fulfilled after some duration (e.g. pin is set high after 10ms), thus events can be dispatched with some delay. Events are considered to be of a lesser priority than tasks. Events can have their own priority to ensure that the most time sensitive events are dispatched first. Note, there is a risk that an event never gets dispatched if the scheduler cannot make time for it.

Overall, this design ensures that tasks are run on time (or creates errors), while allowing for events to be dynamically created and dispached without disrupting the timed tasks. The benefits of this design is that it avoids the issues that arise from having multiple processes executing in parallel, while still providing the benefits of a RTOS.

## Future Work

There's a variety of improvements that could be made to this design. Primarially, the onus is on the user to ensure that each task is offset by the correct amount to ensure that there isn't any overlap. This could be resolved by having the user instead describe dependencies between tasks or a desired order of execution, which could then be topologically sorted to create an order of exeuction that obeyed the dependencies.

Error handling could be improved dramatically to make it more clear which was the task which ran beyond it's expected runtime. Additionally, more work could be done to expand the capability of the error handler, by providing support for different handlers for different tasks or the severity of the error.

Furthermore, more thought should be put into when the error handlers should run. By running the error handler right away, the task which is running late is further delayed, but the error is handled immediately. It would make sense if this was customizable to allow for certain errors to be handled at a higher priority than periodic tasks, and others to be treated like events.

An error should be emitted when an event would never be scheduled to run. This would provide the user with further insights into why the system may not be behaving as expected. Additionally, events currently allocate a small amount of memory, and thus if an event would not get consumed, it's possible for the memory consumption bulid over time. This is expecially concerning on an embedded deivce, where resources are much more limited. This should also be changed, such that there can only be some fixed number of events at a given time, and errors are created if too many events are in the queue at once.

I'd love to come back to this project at some point and re-vamp it and fix these issues. I think there's some potential in the design presented, and with some more work it could be quite practical and useful.
