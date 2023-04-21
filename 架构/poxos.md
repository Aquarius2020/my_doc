#### 什么是Paxos算法？ 如何实现的？

Paxos算法是Lamport宗师提出的一种基于消息传递的分布式一致性算法，使其获得2013年图灵奖。

- **三个角色**？ 可以理解为人大代表(Proposer)在人大向其它代表(Acceptors)提案，通过后让老百姓(Learner)落实

Paxos将系统中的角色分为`提议者 (Proposer)`，`决策者 (Acceptor)`，和`最终决策学习者 (Learner)`:

1. `Proposer`: 提出提案 (Proposal)。Proposal信息包括提案编号 (Proposal ID) 和提议的值 (Value)。
2. `Acceptor`: 参与决策，回应Proposers的提案。收到Proposal后可以接受提案，若Proposal获得多数Acceptors的接受，则称该Proposal被批准。
3. `Learner`: 不参与决策，从Proposers/Acceptors学习最新达成一致的提案(Value)。

在多副本状态机中，每个副本同时具有Proposer、Acceptor、Learner三种角色。

- **基于消息传递的3个阶段**

1. 第一阶段: Prepare阶段

   ；Proposer向Acceptors发出Prepare请求，Acceptors针对收到的Prepare请求进行Promise承诺。 

   1. `Prepare`: Proposer生成全局唯一且递增的Proposal ID (可使用时间戳加Server ID)，向所有Acceptors发送Prepare请求，这里无需携带提案内容，只携带Proposal ID即可。

   2. ```
      Promise
      ```

      : Acceptors收到Prepare请求后，做出“两个承诺，一个应答”。 

      1. 承诺1: 不再接受Proposal ID小于等于(注意: 这里是<= )当前请求的Prepare请求;
      2. 承诺2: 不再接受Proposal ID小于(注意: 这里是< )当前请求的Propose请求;
      3. 应答: 不违背以前作出的承诺下，回复已经Accept过的提案中Proposal ID最大的那个提案的Value和Proposal ID，没有则返回空值。

2. 第二阶段: Accept阶段

   ; Proposer收到多数Acceptors承诺的Promise后，向Acceptors发出Propose请求，Acceptors针对收到的Propose请求进行Accept处理。 

   1. `Propose`: Proposer 收到多数Acceptors的Promise应答后，从应答中选择Proposal ID最大的提案的Value，作为本次要发起的提案。如果所有应答的提案Value均为空值，则可以自己随意决定提案Value。然后携带当前Proposal ID，向所有Acceptors发送Propose请求。
   2. `Accept`: Acceptor收到Propose请求后，在不违背自己之前作出的承诺下，接受并持久化当前Proposal ID和提案Value。

3. **第三阶段: Learn阶段**; Proposer在收到多数Acceptors的Accept之后，标志着本次Accept成功，决议形成，将形成的决议发送给所有Learners。

