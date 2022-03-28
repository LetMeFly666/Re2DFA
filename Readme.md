<!--
 * @Author: LetMeFly
 * @Date: 2022-03-28 15:29:51
 * @LastEditors: LetMeFly
 * @LastEditTime: 2022-03-28 18:42:28
-->
# Re2DFA

Regular expression to DFA(Deterministic Finite State)

## 实现目标

## 概念

+ [正则表达式](#RegularExpression)
+ [字符](#Character)
+ [实字符](#CharacterReal)
+ [空字符](#CharacterEmpty)
+ [运算符](#Operator)
+ [选择符](#OperatorOr)
+ [连接符](#OperatorConcatenation)
+ [重复符](#OperatorRepetition)

**@Input**

| 名称       | 类型   | 描述                                           |
| :--------- | ------ | ---------------------------------------------- |
| 正则表达式<a id="RegularExpression"></a> | String | 代表正则表达式的字符串，包括数个[字符](#Character)和[运算符](#Operator)。 |

**字符**

| 名称 | 类型 | 描述                                                         |
| ---- | ---- | ------------------------------------------------------------ |
| 字符<a id="Character"></a> | Char | 代表串中要出现的字符<br/>•支持**实字符**<a id="CharacterReal"></a>```a~z```、```A~Z```、```0~9```<br/>•支持**空字符**<a id="CharacterEmpty"></a>```ε``` |

**运算符**

| 名称   | 类型     | 描述 |
| ------ | -------- | ---- |
| 运算符<a id="Operator"></a> | Operator | 确定正则的运算规则<br/>•支持<a href="#OperatorOr">**选择符**</a><code>\|</code><br/>•支持<a href="#OperatorConcatenation">**连接符**</a>```·```<br/>•支持<a href="#OperatorRepetition">**重复符**</a>```*``` |

| 名称 | 类型 | 描述 |
| ---- | --- | --- |
| 选择符<a id="OperatorOr"></a> | Operator | 若```a```和```b```代表两个正则表达式，则```a|b```代表```a或b```，即无论是```a```还是```b```都能匹配正则表达式 |
| 连接符<a id="OperatorConcatenation"></a> | Operator | 若```a```和```b```代表两个正则表达式，则```a·b```代表```a后b```（```·```可省略，```a·b```等价于```ab```），即若一个串能从某处分成两串，使得前串匹配```a```且后串匹配```b```，则此串能匹配```a·b``` |
| 重复符<a id="OperatorRepetition"></a> | Operator | 若```a```代表一个正则表达式，则```a*```代表```数个a```（个数n≥0），即若一个串能分成数个串，使得每个串都匹配```a```，则此串能匹配```a*``` |
