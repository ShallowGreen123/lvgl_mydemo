
# 1、简介
LVGL 虽然提供了 30+ 的组件，和上百种的样式，还支持之定义组件，但是没有界面切换的接口。在用户与屏幕交互的过程中，用户与屏幕的交互并不总是在同一位置开始，而是经常以不确定的方式开始。例如，如果您从主屏幕打开电子邮件应用，可能会看到电子邮件列表，如果您通过社交媒体应用启动电子邮件应用，则可能会直接进入电子邮件应用的邮件撰写界面。

例如如下图，当前屏幕在 scr4，用户返回是要做一个判断到底是返回到 scr2呢，还是返回到 scr3 呢，当界面关系比较复杂的时候，这样的判断就显得很鸡肋了。

 ![[Pasted image 20230221105527.png]]

假设从 scr1 -> scr3 -> scr4 到达当前屏幕 scr4，当想返回 scr1 时，是按照 scr4 -> scr3 -> scr1 的顺序依次退出，直到回到 scr1。这样先进后出的特性与栈的特性非常相似，所以可以使用栈结构来实现界面管理的功能。


# 2、实现
运用栈的原理，定义界面生命周期，定义界面处理结构体 handle。

## 2.1 界面的生命周期
单个界面 ==**scr_handle**== 的定义如下，创建界面，进入界面，更新界面布局，退出界面，销毁界面。
~~~c
struct scr_handle {
    uint32_t ScrId;
    lv_obj_t *(*ScrCreate)(lv_obj_t *parent);
    void (*ScrEnter)(void);
    void (*ScrExit)(void);
    void (*ScrDestroy)(void);
}
~~~

~~~
create -> entry -> layout -> exit -> destory
		   ^                   |
		   |___________________|
~~~

==**需要注意的是，create 和 destroy、entry 和  exit 为对称结构，也就是说，再create申请的资源需要在 destroy 中释放，同样的在 entry 中申请的资源需要在 exit 中释放。**==

| interface   | Description |
| ----------- | ----------- |
| Create      | 界面 obj 的创建、各种组件的创建、定时器创建 |
| Entry       | 为组件添加事件，开启定时器，注册监听器      |
| Layout      | 对界面进行布局，界面处于活跃状态            |
| Exit        | 屏蔽组件事件，暂停定时器，注销监听器        |
| Destroy     | 销毁组件、销毁定时器、销毁界面              |

## 2.2 界面状态
单个界面状态 ==**scr_state**== 的定义如下，空闲态，销毁态，创建态，非活跃态，活跃态。

| State       | Description |
| ----------- | ----------- |
| idle      | Not in use                           |
| destory   | Not active and having been destroyed |
| create    | Created                              |
| inactive  | Not active                           |
| activebg  | Active but at the background         |
| active    | Active and at the foreground         |

## 2.3 界面 list 和 stack

==**Scr_List**== 是一个全局变量，每添加一个界面需要在 Scr_List 注册一下；==**Scr_Stack**== 记录在界面的跳转关系，与简介上面的例子一样，当想回到 scr1，只需要依次退出 scr4、scr3 就可以回到 scr1了。这样的好处就是：当界面很多时，也能过维持这样先进后出的关系。

![[Pasted image 20230221112554.png]]
==**Scr_Stack**== 的定义如下
~~~c
struct Scr_Stack{
	struct scr_handle scr_handel;
	lv_obj_t *Obj;
	uint32_t state;
	struct Scr_Stack *pnext;
}
~~~

# 3、API

~~~c
/* config */
#define scr_switch_time 500                        /* 动画持续时间 */
#define scr_switch_anim_delay 0                    /* 动画播放前的延时 */
#define scr_switch_anim LV_SCR_LOAD_ANIM_MOVE_LEFT /* 界面切换动画 */

/* function */
void ScrMgeInit(void);/* 初始化Screen List 和Screen Stack */
int ScrMgeSwitchTo(uint32_t id, bool anim);/* 切换当前界面到第 id 个界面上*/
int ScrMgePush(uint32_t id, bool anim);    /* 向栈顶插入一个界面 */
int ScrMgePop(bool anim);                  /* 弹出栈顶界面 */
int ScrMgePopNScr(uint32_t n, bool anim);  /* 弹出n个界面 */
int ScrMgePopToRoot(bool anim);            /* 弹出到栈底 */
~~~

# 4、例子

下面例子列举了两个界面，scr1 和 scr2 界面应按如下方式定义；下面例子的功能为，scr1为主界面，点击界面上的按键，进入 scr2 界面，然后再 scr2 点击按键退回到 scr1。
~~~c
/*** screen1.c ***/
static obj_event_cb2(lv_event_t *e){
	if(event == btn){
		ScrMgePush(ID_SCREEN2, true); // 跳转到 Screen2
	}
}

lv_obj_t *CreateScr1(lv_obj_t *parent){
	lv_obj_t *Obj = lv_obj_craete(parent);
	/* User code */
	lv_obj_t *btn = lv_btn_create(Obj);
	...
	/* User code end */
	return Obj;
}
static void EntryScr1(void){/* User code */}
static void LayoutScr1(void){/* User code */}
static void ExitScr1(void){/* User code */}
static void DestoryScr1(void){/* User code */}

const scr_handle scr1_item = {
    .ScrId      = ID_SCREEN1,
    .ScrCreate  = CreateScr1,
    .ScrEnter   = EntryScr1,
    .ScrExit    = LayoutScr1,
    .ScrDestroy = DestoryScr1,
};

/*** screen2.c ***/
static obj_event_cb2(lv_event_t *e){
	if(event == btn){
		ScrMgePop(true); // 回退到 Screen1
	}
}

lv_obj_t *CreateScr2(lv_obj_t *parent){
	lv_obj_t *Obj = lv_obj_craete(parent);
	/* User code */
	lv_obj_t *btn = lv_btn_create(Obj);
	...
	/* User code end */
	return Obj;
}
static void EntryScr2(void){/* User code */}
static void LayoutScr2(void){/* User code */}
static void ExitScr2(void){/* User code */}
static void DestoryScr2(void){/* User code */}

const scr_handle scr2_item = {
    .ScrId      = ID_SCREEN2,
    .ScrCreate  = CreateScr2,
    .ScrEnter   = EntryScr2,
    .ScrExit    = LayoutScr2,
    .ScrDestroy = DestoryScr2,
};

/*** scr_mge.c ***/
static const scr_list_item *Scr_List= {
       &scr1_item,
       &scr2_item,
};

/*** scr_mge.h ***/
enum{
	ID_SCREEN1 = 1,
	ID_SCREEN2,
}
extern const scr_handle scr1_item;
extern const scr_handle scr2_item;

/*** main.c ***/
int main(){
	ScrMgeInit(); 
	ScrMgeSwitchTo(ID_SCREEN1, true);   /* 第一调用该函数为设置主界面，     */
										/* 因为 Scr_Stack 至少要有一个元素 */
}
~~~

