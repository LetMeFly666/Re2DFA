<!--
 * @Author: LetMeFly
 * @Date: 2022-03-28 15:29:51
 * @LastEditors: LetMeFly
 * @LastEditTime: 2022-03-31 10:01:07
-->
# Re2DFA

Regular expression to DFA(Deterministic Finite State)

[项目地址](https://github.com/LetMeFly666/Re2DFA)

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
| 运算符<a id="Operator"></a> | Operator | 确定正则的运算规则<br/>•支持<a href="#OperatorOr"><font color="black"><b>选择符</b></font></a><code>&#124;</code><br/>•支持<a href="#OperatorConcatenation"><font color="black"><b>连接符</b></font></a>```·```<br/>•支持<a href="#OperatorRepetition"><font color="black"><b>重复符</b></font></a>```*``` |

| 名称 | 类型 | 描述 |
| ---- | --- | --- |
| 选择符<a id="OperatorOr"></a> | Operator | 若```a```和```b```代表两个正则表达式，则<code>a&#124;b</code>代表```a或b```，即无论是```a```还是```b```都能匹配正则表达式 |
| 连接符<a id="OperatorConcatenation"></a> | Operator | 若```a```和```b```代表两个正则表达式，则```a·b```代表```a后b```（```·```可省略，```a·b```等价于```ab```），即若一个串能从某处分成两串，使得前串匹配```a```且后串匹配```b```，则此串能匹配```a·b``` |
| 重复符<a id="OperatorRepetition"></a> | Operator | 若```a```代表一个正则表达式，则```a*```代表```数个a```（个数n≥0），即若一个串能分成数个串，使得每个串都匹配```a```，则此串能匹配```a*``` |

## 实现思路

输入的正则表达式 → 添加上省略的· → 转换为逆波兰表达式 → 转为NFA → 转为DFA

### 添加上省略的·

有以下情况需要在中间添加·：

```·```：194&#124;183

```ε```：206&#124;181

程序内部用```.```代表```·```，用```,```代表```ε```

+ ab
+ a(
+ )a

## Release

### How To Release

#### 打包QT程序所需依赖

```bat
windeployqt Re2DFA.exe
```

其中 ```windeployqt``` 可以直接打开```QT的命令行```来使用。或者找到自己电脑上```windeployqt.exe```的位置。例如我电脑```QT安装目录```是```F:\OtherApps\Program\QT\Apps```，安装版本是```msvc2017 x64 5.14.2```，那么我电脑上```windeployqt.exe```就在：```F:\OtherApps\Program\QT\Apps\5.14.2\msvc2017_64\bin\windeployqt.exe```

```bat
"F:\OtherApps\Program\QT\Apps\5.14.2\msvc2017_64\bin\windeployqt.exe" Re2DFA.exe
```