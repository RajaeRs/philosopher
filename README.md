# philosopher
## Threads :

### Data types:

The Pthreads API defines a number of data types. *SUSv3* doesn’t specify how these data types should be represented, and portable programs should treat them as opaque data. By this, we mean that a program should avoid any reliance on knowledge of the structure or contents of a variable of one of these types. In particular, we can’t compare variables of these types using the C *==* operator.

### Errno :

In the traditional UNIX API, *errno* is a global integer variable. However, this doesn’t suffice for threaded programs. If a thread made a function call that returned an error in a global *errno* variable, then this would confuse other threads that might also be making function calls and checking *errno*. In other words, race conditions would result. Therefore, in threaded programs, each thread has its own *errno* value. On Linux, a thread-specific *errno* is achieved in a similar manner to most other UNIX implementations: *errno* is defined as a macro that expands into a function call returning a modifiable lvalue that is distinct for each thread. (Since the lvalue is modifiable, it is still possible to write assignment statements of the form *errno = value* in threaded programs.)

### Return value:

All Pthreads functions return 0 on success or a positive value on failure. The failure value is one of the same values that can be placed in *errno* by traditional UNIX system calls.

### Compiling :

On Linux, programs that use the Pthreads API must be compiled with the *cc –pthread* option. The effects of this option include the following:

- The _REENTRANT preprocessor macro is defined. This causes the declarations of a few reentrant functions to be exposed.
- The program is linked with the *libpthread* library (the equivalent of *–lpthread*).

### Thread Fonctions:

When a program is started, the resulting process consistes of single thread, called the initial or main thread.  

**pthread_create :** 

```c
#include <pthread.h>

int pthread_create(pthread_t *thread, const pthread_attr_t *attr,
                   void *(*start_routine)(void*), void *arg);
/*
-> return : 0 on success ; positive error number on error

-> thread: A pointer to a pthread_t object where the newly created thread
	ID will be stored. 

-> attr: A pointer to a pthread_attr_t object that specifies the attributes
	of the newly created thread.

-> start_routine: A function pointer to the function that will be executed
	by the new thread. This function should have the following signature:
	void* function_name(void*). The function takes a void* argument
	and returns a void* result.

-> arg: A pointer to the argument that pass to the start_routine function.
	This argument needs to be cast to void* when passed.
*/
```

**pthread_join :**  Function waits for the thread identified by thread to terminate.

```c
int pthread_join(pthread_t thread, void **value_ptr);

/*
-> return : returns zero on success and an error code otherwise.

-> thread: The thread identifier (pthread_t) of the thread that the calling 
	thread wants to join. \

-> value_ptr: Double pointer (void **) to a location where
	the exit status of the joined thread can be stored.
	The exit status is a value returned by the thread function when it exits.
	If the exit status is not required, this parameter can be set to NULL.
*/
```

**pthread_detach :** By default, a thread is *joinable*, meaning that when it terminates, another thread can obtain its return status using *pthread_join()*. Sometimes, we don’t care about the thread’s return status; we simply want the system to automatically clean up and remove the thread when it terminates. In this case, we can mark the thread as *detached*, by making a call to *pthread_detach()* specifying the thread’s identifier in *thread*.

```c
int pthread_detach(pthread_t thread);

/*
-> return : returns zero on success and an error code otherwise.

-> thread: The thread identifier that to be detached
*/
```

**Pthread Mutex :**

To avoid the problems that can occur when threads try to update a shared variable, we must use a *mutex* (short for *mutual exclusion*) to ensure that only one thread at a time can access the variable.
A mutex has two states: *locked* and *unlocked*. At any moment, at most one thread may hold the lock on a mutex. Attempting to lock a mutex that is already locked either blocks or fails with an error. When a thread locks a mutex, it becomes the owner of that mutex. Only the mutex owner can unlock the mutex.

**pthread_mutex_lock :** To lock a mutex, we specify the mutex in a call to *pthread_mutex_lock()*. If the mutex is currently unlocked, this call locks the mutex and returns immediately. If the mutex is currently locked by another thread, then *pthread_mutex_lock()* blocks until the mutex is unlocked, at which point it locks the mutex and returns.

```c
int pthread_mutex_lock(pthread_mutex_t *mutex);

// -> return : returns zero on success and an error code otherwise.
```

**pthread_mutex_unlock :** function unlocks a mutex previously locked by the calling thread. It is an error to unlock a mutex that is not currently locked, or to unlock a mutex that is locked by another thread.

```c
int pthread_mutex_unlock(pthread_mutex_t *mutex);

// -> return : returns zero on success and an error code otherwise.
```

**pthread_mutex_init :** The static initializer value PTHREAD_MUTEX_INITIALIZER can be used only for initializing a statically allocated mutex with default attributes. In all other cases, we must dynamically initialize the mutex using *pthread_mutex_init()*.

```c
int pthread_mutex_init(pthread_mutex_t *mutex, const pthread_mutexattr_t *attr);

/*
-> return : returns zero on success and an error code otherwise.

-> mutex: This is a pointer to a pthread_mutex_t variable that represents 
	the mutex to be initialized. The pthread_mutex_t type is a structure
	that holds the internal data for the mutex.

-> attr: This is an optional pointer to a pthread_mutexattr_t structure
	that specifies the attributes of the mutex. Attributes allow you to customize
	the behavior of the mutex. If you want to use the default attributes,
	you can pass NULL for this parameter. The pthread_mutexattr_t type is also
	a structure used for defining mutex attributes.
*/
```

**pthread_mutex_destroy:** 1. A mutex that has been destroyed with *pthread_mutex_destroy()* can subsequently be reinitialized by *pthread_mutex_init()*.

```c

```

### Time :

**gettimeofday :** System call returns the calendar time in the buffer pointed to by *tv.*

```c
#include <sys/time.h>

int gettimeofday(struct timeval *tv, struct timezone *tz);

/* -> return : 0 on success , -1 on error. */

/* -> timeval :  */
struct timeval {
		time_t tv_sec;
		suseconds_t tv_usec;
	/*
		tv_sec : Seconds since 00:00:00, 1 Jan 1970 UTC
		tv_usec : Additional microseconds (long int)
	*/
};

/*
-> tz : Is a historical artifact. In older UNIX imple- mentations, it was used
	to retrieve timezone information for the system. This argu- ment is now
	obsolete and should always be specified as NULL.
*/
```

## Processes :

### File Sharing Between Parent and Child :

When a *fork()* is performed, the child receives duplicates of all of the parent’s file descriptors. These duplicates are made in the manner of *dup()*, which means that corresponding descriptors in the parent and the child refer to the same open file description.

![Screen Shot 2023-06-02 at 4.34.01 PM.png](https://s3-us-west-2.amazonaws.com/secure.notion-static.com/24e1a4ca-06fc-409c-aed6-22fefecab437/Screen_Shot_2023-06-02_at_4.34.01_PM.png)

### The wait status value:

The *status* value returned by *wait()* and *waitpid()* allows us to distinguish the following events for the child:

- The child terminated by calling *_exit()* (or *exit()*), specifying an integer *exit status*.
- The child was terminated by the delivery of an unhandled signal.
- The child was stopped by a signal, and *waitpid()* was called with the WUNTRACED flag.
- The child was resumed by a SIGCONT signal, and *waitpid()* was called with the `WCONTINUED` flag.

The statement you provided refers to the usage of the **`status`** parameter in a specific context, likely related to process management or system calls. Let's break down the explanation:

1. **`status`** as an **`int`**: The **`status`** parameter is defined as an **`int`** data type. In most programming languages, an **`int`** typically occupies 4 bytes of memory.
2. Only bottom 2 bytes used: Despite being defined as an **`int`**, only the bottom 2 bytes of the value pointed to by **`status`** are actually used. This means that only the last 16 bits (2 bytes) of the **`status`** value hold the relevant information, while the upper 2 bytes are disregarded or unused.
3. Interpretation based on events: The exact value and interpretation of the bottom 2 bytes of **`status`** depend on the specific events that occurred for the child process. Without further context, it's challenging to provide precise details. However, in process management scenarios, such as when using system calls like **`fork()`** or **`wait()`**, these bits may contain information about the child process's exit status, termination cause, or other event-specific details.
4. Filling of the 2 bytes: How these 2 bytes are filled depends on the specific events that occurred for the child process. Different events may set different bits or combinations of bits in the 2 bytes to represent different states or outcomes. The exact mechanism of filling these bits is typically defined by the operating system or the specific context in which **`status`** is used.

![Screen Shot 2023-06-02 at 5.43.25 PM.png](https://s3-us-west-2.amazonaws.com/secure.notion-static.com/6c67a5ef-3599-4537-9fdc-0c834575b62d/Screen_Shot_2023-06-02_at_5.43.25_PM.png)

### Childe process non termination befor it’s parent :

1. Orphaned Child Process: If a child process is still running when its parent process terminates, it becomes an orphan. In this case, the operating system adopts the orphaned child process and assigns it to the init process. The init process has a process ID of 1 and serves as the root of all processes in the system.
2. Zombie Process: When a child process terminates, it enters a state called a zombie process. A zombie process is a terminated process that still has an entry in the kernel's process table. The entry contains information about the process's termination status and other necessary details.
3. Reaping Zombie Processes: Zombie processes are not automatically removed from the system. They remain in the process table until their parent process calls a function like wait() or a similar mechanism to retrieve the termination status of the child process. By calling wait(), the parent process can collect the exit status of the terminated child process and perform necessary cleanup operations.
4. Importance of Reaping: It's essential for long-running programs like shells or daemons to design their code in a way that ensures they always reap the status of their child processes. If zombie processes are left unreaped, they accumulate in the kernel's process table over time. This can lead to resource exhaustion and a degradation in system performance.
5. Zombie State and Killing: A process in the zombie state cannot be killed or terminated directly. It has already completed its execution, and its termination status is stored in the process table. To remove the zombie process from the system, its parent process needs to call wait() or a similar mechanism.

### Posix semaphores :

In POSIX, A semaphore is a synchronization primitive used in concurrent programming to control access to shared resources or coordinate the execution of multiple processes or threads. It is typically an integer variable with specific operations defined on it.

The primary purpose of a semaphore is to enforce mutual exclusion, ensuring that only one process or thread can access a shared resource at a time. It helps prevent race conditions and data inconsistencies that can occur when multiple entities try to access and modify a shared resource concurrently.

**Type :**

- Named semaphores: **This type of semaphore has a name. By calling *sem_open()* with the same name, unrelated processes can access the same semaphore.
- Unnamed semaphores: also known as anonymous semaphores, are a synchronization mechanism used in concurrent programming. They differ from named semaphores in that they don't have an explicit name associated with them. Instead, they are typically located at a predetermined memory location.
- Deffrence **:** named semaphores are used for communication and synchronization between independent processes, offering system-wide visibility and persistence. On the other hand, unnamed semaphores are primarily used for synchronization within a single process, facilitating coordination between threads without the need for explicit naming or system-wide accessibility.
    - More ;
        1. **Naming and Identification:**
            - Named Semaphores: Named semaphores have an explicit name associated with them. Processes or threads can access a named semaphore by using its unique name, allowing for system-wide visibility and access.
            - Unnamed Semaphores: Unnamed semaphores do not have an explicit name. Instead, they are located at a predefined memory location, known to the processes or threads that need to access them. They rely on shared memory or shared variables to establish communication or synchronization.
        2. **Scope:**
            - Named Semaphores: Named semaphores are typically used for inter-process communication (IPC), allowing synchronization and coordination across multiple processes. They can be accessed by different processes running on the same machine or even on different machines connected over a network.
            - Unnamed Semaphores: Unnamed semaphores are often used for intra-thread synchronization within a single process. They facilitate coordination between multiple threads sharing the same memory space.
        3. **Persistence:**
            - Named Semaphores: Named semaphores can persist beyond the lifetime of the processes or threads that created them. Other processes or threads can open the named semaphore and use it even after the original creator has terminated.
            - Unnamed Semaphores: Unnamed semaphores exist only as long as the processes or threads that create them are active. Once the processes or threads terminate, the unnamed semaphores are destroyed and no longer accessible.
        4. **Creation and Management:**
            - Named Semaphores: Creating and managing named semaphores typically involve specific system calls or functions provided by the operating system or concurrency libraries. These calls allow creating, opening, and closing named semaphores.
            - Unnamed Semaphores: Unnamed semaphores are often created and managed through shared memory mechanisms or shared variables within the application's code. They don't require explicit system calls to create or manage the semaphore itself.
        

**Opening ,`sem_open()`:**
The *sem_open()* function creates and opens a new named semaphore or opens an existing semaphore.

```c
sem_t *sem_open(const char *name, int oflag, ...);

/*(...) mode_t mode, unsigned int value : are optional */
/*  return : Returns pointer to semaphore on success, or SEM_FAILED on error. */
```

- `name :` The name argument identifies the semaphore. must be unique and started
	with "/".
- `oflag`: is a bitmask that determines the behavior of opening or creating a semaphore. It controls whether you are opening an existing semaphore or creating a new semaphore, and it can take several possible values
    - **more:**
        - **Is set to 0**, it indicates that you are trying to open an existing semaphore. In this case, **`sem_open()`** will attempt to find a semaphore with the given name. If a semaphore with the specified name exists, it will be opened, and a valid semaphore descriptor (**`sem_t*`**) will be returned. If no semaphore with the given name exists, **`sem_open()`** will fail and return **`SEM_FAILED`**.
        - I**ncludes the `O_CREAT` flag,** it indicates that you want to create a new semaphore if one with the given name doesn't already exist. In this case, **`sem_open()`** will attempt to create a new semaphore. If a semaphore with the specified name already exists, the **`O_CREAT`** flag has no effect, and **`sem_open()`** will open the existing semaphore instead of creating a new one. If the semaphore is successfully created or opened, a valid semaphore descriptor will be returned.
        - **Includes both `O_CREAT` and `O_EXCL` flags,** it indicates that you want to create a new semaphore exclusively. In other words, you want to ensure that a semaphore with the given name does not already exist. If a semaphore with the specified name exists, **`sem_open()`** will fail and return **`SEM_FAILED`**. This flag is useful to avoid accidentally overwriting an existing semaphore.
- 

**Named Semaphor :**

- The *sem_open()* function opens or creates a semaphore, initializes the sema-
phore if it is created by the call, and returns a handle for use in later calls.
- The *sem_post(sem)* and *sem_wait(sem)* functions respectively increment and dec-
rement a semaphore’s value.
- The *sem_getvalue()* function retrieves a semaphore’s current value.
- The *sem_close()* function removes the calling process’s association with a sema-
    
    phore that it previously opened.
    
- The *sem_unlink()* function removes a semaphore name and marks the sema-
phore for deletion when all processes have closed it.

When a process wants to access a shared resource protected by a semaphore, it typically follows these steps:

1. Attempt to decrease the value of the semaphore (decrement) by calling a function such as **`sem_wait()`** or **`sem_trywait()`**.
2. If the current value of the semaphore is greater than 0, the decrement operation succeeds, and the process can proceed with accessing the shared resource.
3. If the current value of the semaphore is 0, indicating that the resource is currently unavailable or all slots are occupied, the behavior depends on the function used:
    - `**sem_wait()**`: This function blocks the calling process until the semaphore value becomes greater than 0. It effectively waits for the resource to become available, putting the process into a waiting state.
    - **`sem_trywait()`**: This function attempts to decrement the semaphore value but returns immediately with an error if the value is currently 0. It does not block the process and allows it to perform other tasks while periodically trying to access the resource.
    - **`sem_timedwait()`**: This function is similar to **`sem_wait()`**, but it includes a timeout parameter. If the semaphore value is 0, the process will block until either the semaphore value becomes greater than 0 or the specified timeout period expires.

These mechanisms help ensure that a process does not access a shared resource when it is not available. By blocking or returning an error, the process can synchronize its access with other processes or threads that may be using the same resource.

Additionally, once a process has finished using the shared resource, it increments the semaphore value (using a function like **`sem_post()`**), indicating that the resource is now available for other processes to use.