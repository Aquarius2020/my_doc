
-- 基本实例就不说了,这里主要说一下窗口的定义
max(id) over (order by column rows between <beg> and <end>)

-- beg 和 end 可以又一下几种
- `UNBOUNDED PRECEDING`：从窗口的开始位置。
- `n PRECEDING`：从当前行的前 `n` 行。
- `CURRENT ROW`：当前行。
- `n FOLLOWING`：从当前行的后 `n` 行。
- `UNBOUNDED FOLLOWING`：到窗口的结束位置。

-- 不一定是 current 两遍扩展, 但是一定是 [beg,end] 构成一个合法区间.
-- 即 以 current 为参考系, beg 在上, end 在下. 反过来不是有效区间. 或者说,行号关系,beg<end
-- 所以,窗口函数能求的更多.
-- 比如,求当前行前 4 行到前 2 行这三行的和. 这是 raws between 4 preceding and 2 preceding 的用法.