-- Active: 1697429850366@@127.0.0.1@3306@vhr

drop Procedure `GET_ALL_TABLES`;
DELIMITER //

CREATE PROCEDURE GET_ALL_TABLES() 
BEGIN 
	-- 声明变量
	declare val int DEFAULT 0;
	show tables;
	-- select 外使用 set
	set val=1;
	-- select 内使用 into
	select count(*) into val from hr;
	select val;
end // 


DELIMITER ;

call GET_ALL_TABLES()


drop PROCEDURE get_number;
DELIMITER //

create Procedure get_number(in v1 int, out o int,INOUT tmp int)
BEGIN
select "before ",o,tmp;
IF v1>0 then
	select count(*) into o from hr where id=v1;
ELSE
	set o=-100;
end IF;

-- 用 if 不如用 case when. 可读性更高
case 
	when v1>0 then select "大于 0";
	when v1<0 then select "小于 0";
	when v1=0 then select "等于 0";
end CASE ;

set tmp=12 ;
select "after ",o,tmp;

END
DELIMITER ;


set @val=1;
set @val2=2;
call get_number(3,@val,@va2);
select @val,@val2;

