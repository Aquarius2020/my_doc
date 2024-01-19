一、什么是 AQS ？

    AQS的全称为（AbstractQueuedSynchronizer）抽象的队列式的同步器，是并发编程中实现同步器的一个框架。字面意思理解：
    
        是一个抽象类，也就是它定义了一些基础功能，会有很多子类去扩展他的功能。
        是一个队列式的，也就是会用来协调多个线程的。
        是一个同步器，那么是用来给多个线程同步某个数据或者某个状态的。
    
    基于AQS实现的Lock, CountDownLatch、CyclicBarrier、Semaphore子类，Lock是锁、CountDownLatch是计数n个线程完成某个目标后进行下一步（相当于阀门）、CyclicBarrier栅栏相当于多个线程一起到某一步后再让他们进行下一步、Semaphore相当于一个令牌桶维护一组许可证可以做限流
实现原理：

    AQS基于一个FIFO双向队列实现，被设计给那些依赖一个代表状态的原子int值的同步器使用。我们都知道，既然叫同步器，那个肯定有个代表同步状态（临界资源）的东西，在AQS中即为一个叫state的int值，该值通过CAS进行原子修改。
    在AQS中存在一个FIFO队列，队列中的节点表示被阻塞的线程，队列节点元素有4种类型，每种类型表示线程被阻塞的原因，这四种类型分别是：
    
        CANCELLED : 表示该线程是因为超时或者中断原因而被放到队列中
        CONDITION : 表示该线程是因为某个条件不满足而被放到队列中，需要等待一个条件，直到条件成立后才会出队
        SIGNAL : 表示该线程需要被唤醒
        PROPAGATE ： 表示在共享模式下，当前节点执行释放release操作后，当前结点需要传播通知给后面所有节点

由于一个共享资源同一时间只能由一条线程持有，也可以被多个线程持有，因此AQS中存在两种模式，如下:

        1、独占模式
        独占模式表示共享状态值state每次只能由一条线程持有，其他线程如果需要获取，则需要阻塞,如JUC中的ReentrantLock
        2、共享模式
        共享模式表示共享状态值state每次可以由多个线程持有，如JUC中的CountDownLatch

二、Node源码

既然AQS是基于一个FIFO队列的框架，那么我们先来看下队列的元素节点Node的数据结构，源码如下：

```java
static final class Node {
    /**共享模式*/
    static final Node SHARED = new Node();
    /**独占模式*/
    static final Node EXCLUSIVE = null;

    /**标记线程由于中断或超时，需要被取消，即踢出队列*/
    static final int CANCELLED =  1;
    /**线程需要被唤醒*/
    static final int SIGNAL = -1;
    /**线程正在等待一个条件*/
    static final int CONDITION = -2;
    /**传播 当前节点执行释放release操作后，需要通知到其他线程*/
    static final int PROPAGATE = -3;
    
    // waitStatus只取上面CANCELLED、SIGNAL、CONDITION、PROPAGATE四种取值之一
    volatile int waitStatus;

    // 表示前驱节点
    volatile Node prev;

    // 表示后继节点
    volatile Node next;

    // 队列元素需要关联一个线程对象
    volatile Thread thread;

    // 表示下一个waitStatus值为CONDITION的节点
    Node nextWaiter;

    /**
     * 是否当前结点是处于共享模式
     */
    final boolean isShared() {
        return nextWaiter == SHARED;
    }

    /**
     * 返回前一个节点，如果没有前一个节点，则抛出空指针异常
     */
    final Node predecessor() throws NullPointerException {
        // 获取前一个节点的指针
        Node p = prev;
        // 如果前一个节点不存在
        if (p == null)
            throw new NullPointerException();
        else
        // 否则返回
            return p;
    }

    // 初始化头节点使用
    Node() {}

    /**
     *  当有线程需要入队时，那么就创建一个新节点，然后关联该线程对象，由addWaiter()方法调用
     */
    Node(Thread thread, Node mode) {     // Used by addWaiter
        this.nextWaiter = mode;
        this.thread = thread;
    }

    /**
     * 一个线程需要等待一个条件阻塞了，那么就创建一个新节点，关联线程对象
     */
    Node(Thread thread, int waitStatus) { // Used by Condition
        this.waitStatus = waitStatus;
        this.thread = thread;
    }
}
```

三、AQS中的共享状态值

```java
/**
 * 同步状态值
 * 使用volatile修饰，保证多线程修改的可见性
 */
private volatile int state;

/**
 * 获取同步状态值
 * 使用final修饰，子类不能覆盖，只能调用
 */
protected final int getState() {
    return state;
}

/**
 * 修改同步状态值
 */
protected final void setState(int newState) {
    state = newState;
}

/**
 * CAS修改state值
 */
protected final boolean compareAndSetState(int expect, int update) {
        // See below for intrinsics setup to support this
        return unsafe.compareAndSwapInt(this, stateOffset, expect, update);
    }
```

四、AQS中的tryAcquire、tryRelease方法：

    AQS类中，tryAcquire()、tryAcquireShared()、tryRelease()和tryReleaseShared()等方法用于获取和释放state，但AQS类这几个方法都是直接抛出了UnsupportedOperationException异常，这里其实是使用了模板模式，子类只需重写这是个方法即可（其实是其中2个，分别对应独占模式和共享模式）。

```java
/**
 * 尝试以独占模式获取。 该方法应该查询对象的状态是否允许以独占模式获取，如果是，则获取它。 
 * 该方法总是由执行获取的线程调用。 如果此方法报告失败，则获取方法可能将线程排队（如果尚未排队），直到被其他线程释放为止。 这可以用于实现方法Lock.tryLock()。 
 */
protected boolean tryAcquire(int arg) {
    throw new UnsupportedOperationException();
}
protected boolean tryRelease(int arg) {
    throw new UnsupportedOperationException();
}
/**
 * 尝试以共享模式获取。 该方法应该查询对象的状态是否允许在共享模式下获取该对象，如果是这样，就可以获取它。 
 * 该方法总是由执行获取的线程调用。 如果此方法报告失败，则获取方法可能将线程排队（如果尚未排队），直到被其他线程释放为止。
 * @return int 失败的负值 如果在共享模式下获取成功但没有后续共享模式获取可以成功，则为零; 
 *（支持三种不同的返回值使得这种方法可以在仅获取有时只能完全执行的上下文中使用。）成功后，该对象已被获取
 */
protected int tryAcquireShared(int arg) {
    throw new UnsupportedOperationException();
}
protected boolean tryReleaseShared(int arg) {
    throw new UnsupportedOperationException();
}
/**
 * 如果同步仅针对当前（调用）线程进行保存，则返回true 。 每次调用不等待AbstractQueuedSynchronizer.ConditionObject方法时都会调用此方法。 （等待方法调用release(int)。 ）
 * @return true如果同步是唯一的; 否则false 
 */
protected boolean isHeldExclusively() {
    throw new UnsupportedOperationException();
}
```

五、FIFO队列维护，入队(enqueue)和出队(dequeue)源码：

```java
public abstract class AbstractQueuedSynchronizer
    extends AbstractOwnableSynchronizer
    implements java.io.Serializable {
	
	//无参构造器，供子类初始化调用
    protected AbstractQueuedSynchronizer() {}
    
    static final class Node {//... 省略... }
    
    private transient volatile Node head;
    private transient volatile Node tail;

	/**
     * Inserts node into queue, initializing if necessary. See picture above.
     * @param node the node to insert
     * @return node's predecessor
     */
	private Node enq(final Node node) {
        for (;;) {
            Node t = tail;
            if (t == null) { // Must initialize
                if (compareAndSetHead(new Node()))
                    tail = head;
            } else {
                node.prev = t;
                if (compareAndSetTail(t, node)) {
                    t.next = node;
                    return t;
                }
            }
        }
    }
    private Node addWaiter(Node mode) {
        Node node = new Node(Thread.currentThread(), mode);
        // Try the fast path of enq; backup to full enq on failure
        Node pred = tail;
        if (pred != null) {
            node.prev = pred;
            if (compareAndSetTail(pred, node)) {
                pred.next = node;
                return node;
            }
        }
        enq(node);
        return node;
    }
	
	/**
     * Sets head of queue to be node, thus dequeuing. Called only by
     * acquire methods.  Also nulls out unused fields for sake of GC
     * and to suppress unnecessary signals and traversals.
     * 设置head就是dequeue方法，并且把相应的应用删除，方便GC时回收
     * @param node the node
     */
    private void setHead(Node node) {
        head = node;
        node.thread = null;
        node.prev = null;
    }
    
    /**
     * Wakes up node's successor, if one exists.
     * 通知节点的继承者
     */
    private void unparkSuccessor(Node node) {
        /*
         * If status is negative (i.e., possibly needing signal) try
         * to clear in anticipation of signalling.  It is OK if this
         * fails or if status is changed by waiting thread.
         */
        int ws = node.waitStatus;
        if (ws < 0)
            compareAndSetWaitStatus(node, ws, 0);//调用的都是Unsafe的native方法，直接通过属性偏移量修改内存中的值

        /*
         * Thread to unpark is held in successor, which is normally
         * just the next node.  But if cancelled or apparently null,
         * traverse backwards from tail to find the actual
         * non-cancelled successor.
         */
        Node s = node.next;
        if (s == null || s.waitStatus > 0) {
            s = null;
            //如果为空或状态是CANCELLED，这从尾部开始向前找
            for (Node t = tail; t != null && t != node; t = t.prev)
                if (t.waitStatus <= 0)
                    s = t;
        }
        if (s != null)
            LockSupport.unpark(s.thread);//唤醒这个线程
    }
    //... 省略 ...
}
```



六、基于AQS实现自定义同步类

Mutex是JDK文档的一个实例：

```java
/**
 * 互斥锁，实现LOCK接口，供外界调用
 */
class Mutex implements Lock, java.io.Serializable {
    // 自定义同步器
    private static class Sync extends AbstractQueuedSynchronizer {
        // 判断是否锁定状态
        protected boolean isHeldExclusively() {
            return getState() == 1;
        }

        // 尝试获取资源，立即返回。成功则返回true，否则false。
        public boolean tryAcquire(int acquires) {
            assert acquires == 1; // 这里限定只能为1个量
            if (compareAndSetState(0, 1)) {//state为0才设置为1，不可重入！
                setExclusiveOwnerThread(Thread.currentThread());//设置为当前线程独占资源
                return true;
            }
            return false;
        }

        // 尝试释放资源，立即返回。成功则为true，否则false。
        protected boolean tryRelease(int releases) {
            assert releases == 1; // 限定为1个量
            if (getState() == 0)//既然来释放，那肯定就是已占有状态了。只是为了保险，多层判断！
                throw new IllegalMonitorStateException();
            setExclusiveOwnerThread(null);
            setState(0);//释放资源，放弃占有状态
            return true;
        }
    }

    // 真正同步类的实现都依赖继承于AQS的自定义同步器！
    private final Sync sync = new Sync();

    //lock<-->acquire。两者语义一样：获取资源，即便等待，直到成功才返回。
    public void lock() {
        sync.acquire(1);
    }

    //tryLock<-->tryAcquire。两者语义一样：尝试获取资源，要求立即返回。成功则为true，失败则为false。
    public boolean tryLock() {
        return sync.tryAcquire(1);
    }

    //unlock<-->release。两者语义一样：释放资源。
    public void unlock() {
        sync.release(1);
    }

    //锁是否占有状态
    public boolean isLocked() {
        return sync.isHeldExclusively();
    }
}
```

除了Mutex，还有以下几个同步类是通过AQS的同步器进行同步管理的，不同的地方在于tryAcquire-tryRelease的实现方式不一样

        ReentrantLock：可重入锁，与mutex一样实现了Lock接口，使用独占模式的同步器。
        CountDownLatch：计数器，使用了共享模式的同步器进行多线程执行控制。
        Semphore：一个计数信号量，维持一个许可证池（只计数）每次执行前获取许可证，执行完释放许可证。类似限流算法中的令牌桶算法。




