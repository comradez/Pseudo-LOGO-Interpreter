#include <iostream>
#include <cstdio>
#include <queue>
#include <cstring>
#include <cctype>
#include <cstdlib>
#include <string>
#include <cmath>
#include "toojpeg.h"
#include "zcy.h"
#include "interface.hpp"

FILE *LOGO;
const double PI = acos(-1);
unsigned char *frame;
unsigned char pic[3][1925][1085]; //画布
double direction = -PI / 2, x_pos, y_pos, counter; //全局方向，全局位置，像素计数器
int x_size, y_size, colors[3], tag_counter, line_counter, thickness = 1, framecnt = 15; //全局画布大小，全局颜色
bool vis_pic[1925][1085], vis = true, frameout = true; //海龟可视性，用于bfs的vis
std::vector<std::string> line_vector_global;
FILE* TEMPOUT = NULL;

char* __itoa(int num, char* str, int radix) {
	char index[] = "0123456789ABCDEF"; //索引表
	unsigned unum; /*中间变量*/
	int i = 0, j, k; /*确定unum的值*/
	if(radix == 10 && num < 0) {
		unum = (unsigned) -num;
		str[i++] = '-';
	} else unum = (unsigned) num;/*其他情况*/
	do { //转换
	str[i++] = index[unum % (unsigned)radix];
	unum /= radix;
	} while(unum);
	str[i] = '\0';
	//逆序
	if(str[0] == '-')
		k = 1;/*十进制负数*/
	else
		k = 0;
 	for(j = k; j <= (i - 1) / 2; j++) {
 		char temp;
		temp=str[j];
		str[j]=str[i-1+k-j];
		str[i-1+k-j]=temp;
	}
	return str;
}

void output(unsigned char ch) { fputc(ch, TEMPOUT); }
void tag() {
	if (frameout) {
		char *index = new char[55];
		__itoa(tag_counter, index, 10);
		tag_counter++;
		std::string name = filepath()+"/pics/output_",ext=".jpg";
		name+=index+ext; //创建下一个文件
		TEMPOUT = fopen(name.c_str(), "wb"); //打开这个文件
		if(frame==NULL) frame = new unsigned char[x_size * y_size * 3];
		memset(frame, 0, sizeof(frame));
		for (int i = 0; i < y_size; i++) {
			for (int j = 0; j < x_size; j++) {
				frame[3 * (i * x_size + j)] = pic[0][j + 1][i + 1];
				frame[3 * (i * x_size + j) + 1] = pic[1][j + 1][i + 1];
				frame[3 * (i * x_size + j) + 2] = pic[2][j + 1][i + 1];
			}
		}
		TooJpeg::writeJpeg(output, frame, x_size, y_size);
		fclose(TEMPOUT);
		std::cerr << tag_counter << std::endl;
	}
}

std::vector<std::string> split_space(int num) {
	std::vector<std::string> line_vector;
	std::string str = line_vector_global[num - 1];
	int len = str.length(), pos = 0;
	while (pos < len) {
		if (str[pos] != ' ') {
			std::string str_temp = "";
			int pos_temp = pos;
			while (pos_temp < len && str[pos_temp] != ' ') {
				if (isspace(str[pos_temp]) == false)
					str_temp += str[pos_temp];
				pos_temp++;
			}
			if (!str_temp.empty())
				line_vector.push_back(str_temp);
			pos = pos_temp;
		} else {
			pos++;
		}
	}

	return line_vector;
}
bool available(int x, int y, int r, int g, int b) { return pic[0][x][y] == r && pic[1][x][y] == g && pic[2][x][y] == b; }
//判断画布的[x][y]坐标的颜色是否为(r, g, b)，用于bfs
void plot(int x, int y) {
	if (x >= 0 && x <= x_size && y >= 0 && y <= y_size) {
		pic[0][x][y] = colors[0];
		pic[1][x][y] = colors[1];
		pic[2][x][y] = colors[2];
	}
}
int od(double x) { return x - int(x) < 0.5 ? int(x) : int(x)  + 1; } //取最近的整数
int get_num(std::string str, int &pos) { //从str字符串的第pos位开始逐位读取一个整数，用于读入，后面init会用到。
	int var = 0;
	while(str[pos] >= '0' && str[pos] <= '9') {
		var = 10 * var + str[pos] - '0';
		pos++;
	} pos++;
	return var;
}

int init(const char file[]) { //读取文件的前三行，完成窗口大小、初始颜色、初始位置的读入和设置
	LOGO = fopen(file, "r");
	char *input = new char[1005];
	while (fgets(input, 1005, LOGO)) {
		std::string input1(input);
		line_vector_global.push_back(input1);
		line_counter++;
		memset(input, 0, sizeof(char) * 1005);
	}
	int ptr = 6;
	x_size = get_num(line_vector_global[0], ptr);
	y_size = get_num(line_vector_global[0], ptr);
	//读入背景颜色
	ptr = 12;
	int tr = get_num(line_vector_global[1], ptr),
	    tg = get_num(line_vector_global[1], ptr),
	    tb = get_num(line_vector_global[1], ptr);
	for (int i = 0; i <= x_size; i++)
		for (int j = 0; j <= y_size; j++) {
			pic[0][i][j] = tr;
			pic[1][i][j] = tg;
			pic[2][i][j] = tb;
		}
	colors[0] = colors[1] = colors[2] = 0; //海龟颜色是(0,0,0)
	//读入初始位置
	ptr = 10;
	x_pos = get_num(line_vector_global[2], ptr);
	y_pos = get_num(line_vector_global[2], ptr);
	ptr = 7;
	framecnt = get_num(line_vector_global[3], ptr);
	frameout = (bool)framecnt;
	return line_counter;
}
//-----要求具有的功能-----
void cloak() { vis = false; } //隐身

void turn(double angle) { //给出一个角度angle，顺时针转angle度
	direction += angle * PI / 180; //顺时针
	while (direction >= 2 * PI)
		direction -= 2 * PI;
	while (direction < 0)
		direction += 2 * PI; //保证direction在[0,2*PI)区间上
}

void color(int r, int g, int b) { //给出颜色分量r, g, b，修改颜色为(r,g,b)且设置为可见
	vis = true;
	colors[0] = r % 256;
	colors[1] = g % 256;
	colors[2] = b % 256;
}

void move(double step) {//传入一个step，海龟在当前方向上前进step长度
	if (step < framecnt && framecnt != 0) {
		counter += step;
		if (counter > framecnt) {
			counter -= framecnt;
			tag();
		}
	}//step小于5的情况，累积直到counter到5为止记录tag
	double x0 = x_pos, y0 = y_pos;
	x_pos = x0 + step * std::cos(direction);
	y_pos = y0 + step * std::sin(direction); //更新位置
	if (vis == false) {
		return; //如果隐身则不画线直接退出
	}
	for (int i = 1; i <= od(step * 100); i++) {
		double x_temp = x0 + i * std::cos(direction) * 0.01, y_temp = y0 + i * std::sin(direction) * 0.01;
		if (thickness > 1) {
			if (std::abs(std::tan(direction)) < 0.01) {
				for (int j = -thickness / 2; j <= thickness / 2; j++) {
					plot(od(x_temp), od(y_temp) + j);
				}
			} else {
				double x_offset = std::abs((double)thickness * std::sin(direction)), y_offset = std::abs((double)thickness * std::cos(direction));
				if (std::tan(direction) >= 0) y_offset *= -1; //x_offset强制为正；斜率为正，y_offset为负，否则y_offset为正
				for (int j = 0; j <= od(x_offset * 100); j++) {
					double x_drawtemp = x_temp - x_offset / 2 + j * 0.01,
					       y_drawtemp = y_temp - y_offset / 2 - j / std::tan(direction) * 0.01;
					plot(od(x_drawtemp), od(y_drawtemp));
				}
			}
		} else {
			plot(od(x_temp), od(y_temp));
		}
		if (step >= framecnt && framecnt != 0) {
			if (i % (framecnt * 100) == 0) {
				tag();
			}
		}//step不小于5的情况，每5个pixel记录一个tag
	}
	if (step >= framecnt && framecnt != 0)
		tag();
}

//y_drawtemp - y_temp / x_drawtemp - x_temp == -1/k == -1 / tan(direction)
//思考：如果这是一条直线，step比较大，那么每5个pixel记录一次
//但如果这是一小段弧，step很小，那么也要记一次/记下step长度（？）

//-----扩展功能-----
void paint() { //将海龟目前所在点以及与其联通的所有同色点修改为全局颜色，类似画图油漆桶
	memset(vis_pic, 0, sizeof(vis_pic));
	int x = od(x_pos), y = od(y_pos);
	int red = pic[0][x][y], green = pic[1][x][y], blue = pic[2][x][y];//red, green, blue为海龟目前所在点的颜色
	int px[4] = {0, -1, 1, 0};
	int py[4] = {1, 0, 0, -1};
	std::queue<std::pair<int,int> > paint_queue;
	paint_queue.push(std::make_pair(x, y));
	vis_pic[x][y] = true;
	while (paint_queue.empty() == false) {
		std::pair<int,int> head_point = paint_queue.front();
		int curr_x = head_point.first, curr_y = head_point.second;
		paint_queue.pop();
		plot(curr_x, curr_y);
		for (int i = 0; i < 4; i++) {
			if (curr_x + px[i] >= 0 && curr_x + px[i] <= x_size && curr_y + py[i] >= 0 && curr_y + py[i] <= y_size) {
				if (available(curr_x + px[i], curr_y + py[i], red, green, blue) && vis_pic[curr_x + px[i]][curr_y + py[i]] == false) {
					paint_queue.push(std::make_pair(curr_x + px[i], curr_y + py[i]));
					vis_pic[curr_x + px[i]][curr_y + py[i]] = true;
				}
			}
		}
	}
	tag();
}

void circle(int dir, double radius) { //drad == 1表示顺时针，否则逆时针，这是个不安全的函数，对，反正错误处理处理不到我。
	double dlength = radius * PI / 180;
	int drad = 1;
	if (dir == 0)
		drad = -1;
	for (int i = 1; i <= 360; i++) {
		move(dlength);
		turn(drad);
	}
}

void arc(int dir, double radius, double angle) { //为啥要和CIRCLE分开嘞？当然不是为了凑行数。
	double dlength = radius * PI / 180;
	int drad = 1;
	if (dir == 0)
		drad = -1;
	for (int i = 1; i <= angle; i++) {
		move(dlength);
		turn(drad);
	}
}

void tree(int num, double trunk_length, double angle, double delta, int dep) { //开叉数，树干长度，开叉角度，每层树枝长度衰减，深度
	if (dep == 0)
		return;
	move(trunk_length);
	double d_angle = angle * 1.0 / (num - 1);
	turn(-0.5 * angle);
	for (int i = 1; i <= num - 1; i++) {
		tree(num, trunk_length * delta, angle, delta, dep - 1);
		turn(d_angle);
	}
	tree(num, trunk_length * delta, angle, delta, dep - 1);
	turn(-0.5 * angle);
	turn(180);
	move(trunk_length);
	turn(180);
}

void pos(double x, double y) {
	counter += 1;
	x_pos = x;
	y_pos = y;
	if (vis == true)
		plot(x, y);
	if (counter > framecnt) {
		counter -= framecnt;
		tag();
	}
}

void arg(double angle) { direction = angle / 180 * PI * (-1); }

void check() {
	if (counter > 0) {
		counter = 0;
		tag();
	}
	FILE* OUT = fopen((filepath()+"/rgb_mat.out").c_str(), "w");
	fprintf(OUT, "%d %d\n", x_size, y_size);
	for (int x = 0; x < x_size; x++) {
		for (int y = 0; y < y_size; y++) {
			fprintf(OUT, "%d %d %d ", pic[2][x][y], pic[1][x][y], pic[0][x][y]);
		}
		fprintf(OUT, "\n");
	}
	fclose(OUT);
	delete[] frame;
	//OUT = fopen("num.out", "w");
	//fprintf(OUT, "%d\n", tag_counter - 1);
	//fclose(OUT);
}

void thick(int t) {
	if(t != 0)
		thickness = abs(t);
}