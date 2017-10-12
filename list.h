# List


#include<stdio.h>
const int maxlist = 100;     //表的最大尺寸
const int success = 0;
const int overflow = 1;
const int underflow = 2;
const int invalid_range = 3;

typedef int listEntry;

class list
{
public:
	int size() const;				//返回表结构里的元素个数
	bool full() const;	//判断表是否已满
	bool empty() const;//判断表是否为空
	void clear();//清空表
	int insert(int position, const listEntry &x); //在表位置position上插入元素x
	int retrieve(int position, listEntry &x) const;//取出表位置position上的元素
	int remove(int position);//删除表位置position上的元素
	int replace(int position, const listEntry&x);// 用x替换表位置position上的元素
	void traverse(void(*visit)(listEntry &));//遍历表里所有元素
	list(void);//构造函数
	~list(void);//析构函数
	
	/*以下为各种查找排序方法*/

	int binarySearch1(const listEntry &target, int &position);//此方法用于数据量较大时
		//表里的元素为非递减排列，在表结构里的bottom和top两个索引之间查找元素target
	int binarySearch2(const listEntry &target, int &position);//此方法用于数据量较小时
		//表里的元素为非递减排列，在表结构里的bottom和top两个索引之间查找元素target
	int hash_insert(const listEntry &x, int &position);		//哈希插入
	int hash_find(const listEntry &x, int &position);		//哈希查找
	void insertionSort();							//插入排序
	void shellSort();	//希尔排序					
	void selectionSort();//选择排序
	void bubbleSort();//冒泡排序
	void mergeSort();//归并排序
	void quickSort(int low, int high);//快速排序
protected:
	int count;
	listEntry data[maxlist];		//使用数组来存放数据元素
	void recursiveMergeSort(int low, int high);
	void merge(int first, int second, int high);
	void swap(int low, int high);
};

int list::size() const				//返回表结构里的元素个数
{
	return count;
}

bool list::full() const				//判断表是否已满
{
	return (count == maxlist);
}

bool list::empty() const			//判断表是否为空
{
	return (count == 0);
}

void list::clear()					//清空表
{
	for (int i = 0; i <= count - 1; i++)
		data[i] = 0;
}

int list::insert(int position, const listEntry &x) //在表位置position上插入元素x
{
	if (full())
		return overflow;
	if (position<0 || position>count)
		return invalid_range;
	for (int i = count - 1; i >= position; i--)
		data[position] = x;
	count++;
	return success;
}

int list::retrieve(int position, listEntry &x) const//取出表位置position上的元素
{
	if (empty())
		return underflow;
	if (position<0 || position>count)
		return invalid_range;
	x = data[position];
	return success;
}

int list::remove(int position)//删除表位置position上的元素
{
	if (empty())
		return underflow;
	if (position<0 || position>count)
		return invalid_range;
	for (int i = position; i <= count - 1; i++)
		data[i] = data[i + 1];
	count--;
	return success;
}

int list::replace(int position, const listEntry&x)//用x替换表位置position上的元素
{
	if (empty())
		return underflow;
	if (position<0 || position>count)
		return invalid_range;
	for (int i = position; i <= count - 1; i++)
		data[position] = x;
	return success;
}

void list::traverse(void(*visit)(listEntry &))//遍历表里所有元素
{
	for (int i = 0; i < count; i++)
		(*visit)(data[i]);
}

list::list(void)
{
	count = 0;
}

list::~list(void)
{
}

int list::binarySearch1(const listEntry &target, int &position)//此方法用于数据量较大时
															   //表里的元素为非递减排列，在表结构里的bottom和top两个索引之间查找元素target
{
	int top = count;
	int bottom = 0;
	while (bottom < top)
	{
		int mid = (bottom + top) / 2;
		if (data[mid] < target)
		{
			bottom = mid + 1;
		}
		else
		{
			top = mid;
		}
		if (top < bottom)
			return overflow;
		else
		{
			position = bottom;
			if (data[position] == target)
			{
				return success;
			}
			else
			{
				return overflow;
			}
		}
	}
}

int list::binarySearch2(const listEntry &target, int &position)//此方法用于数据量较小时
															   //表里的元素为非递减排列，在表结构里的bottom和top两个索引之间查找元素target
{
	int top = count;
	int bottom = 0;
	while (bottom < top)
	{
		int mid = (top + bottom) / 2;	//获得中间点
		if (data[mid] == target)
		{
			position = mid + 1;
			return success;
		}
		else if (data[mid]<target)		//中间点元素小于目标元素，将查找范围缩小到右边半个表
		{
			bottom = mid + 1;
		}
		else							//中间点元素大于等于目标元素，将查找范围缩小到左边半个表
		{
			top = mid - 1;
		}
	}
	return overflow;
}

int list::hash_insert(const listEntry &x, int &position)		//哈希插入
{
	int i = x % maxlist;//很简单的求模运算，如果数据量大的话，需要将求模的值进行适当调整
						//必要的时候可以使用二次求模来避免碰撞
	if (data[i] == 0)
	{
		data[i] = x;
		position = i;
		return success;
	}
	else
	{
		return overflow;
	}
}

int list::hash_find(const listEntry &x, int &position)			//哈希查找
{
	int i = x % maxlist;
	if (data[i] == x)
	{
		position = i;
		return success;
	}
	else
	{
		return overflow;
	}
}

void list::insertionSort()								//插入排序
{
	listEntry key;
	int i, j;
	for (j = 1; j < count; j++)							//表头元素看做事已排序部分，其他元素为未排序部分
	{
		key = data[j];									//获取未排序部分的第一个元素，作为待排序元素
		i = j - 1;
		while (i >= 0 && data[i] > key)					//找出待排序元素的插入位置
		{
			data[i + 1] = data[i];
			i = i - 1;
		}
		data[i + 1] = key;								//插入待排序元素
	}
}


void list::shellSort()
{
	int increment;										//记录增量的变量
	int start;											//记录每次插入排序的起始元素位置
	increment = count;
	for (increment = increment / 3; increment > 0; increment = increment / 3)
	{
		for (start = increment; start<count; start++)
			for (int j = start - increment; j >= 0 && data[j] > data[j + increment]; j -= increment)
			{
				swap(data[j], data[j + increment]);
			}
	}
}

void list::selectionSort()
{
	int i, j, min;
	listEntry temp;
	for (j = 0; j < count; j++)
	{
		min = j;
		bool check = false;
		for (i = j + 1; i < count; i++)
		{
			if (data[i] < data[min])
			{
				min = i;
				check = true;
			}
		}
		if (check)
			continue;
		temp = data[j];
		data[j] = data[min];
		data[min] = temp;
	}
}

void list::bubbleSort()
{
	int i, j;
	listEntry temp;
	for (i = 0; i < count - 1; i++)
		for (j = 0; j < count - 1 - i; j++)
			if (data[j + 1] < data[j])
			{
				temp = data[j];
				data[j] = data[j + 1];
				data[j + 1] = temp;
			}
}

void list::mergeSort()
{
	recursiveMergeSort(0, count - 1);
}

void list::quickSort(int low, int high)
{
	if (low > high)
		return;
	int i, j;
	listEntry pivot;
	pivot = data[low];
	i = low;
	j = high;
	while (i != j)
	{
		while (data[j] >= pivot&&i < j)
			j--;
		while (data[i] <= pivot&&i < j)
			i++;
		if (i < j)
			swap(data[i], data[j]);
	}
	data[low] = data[i];
	data[i] = pivot;
	quickSort(low, i - 1);
	quickSort(i + 1, high);
}

void list::recursiveMergeSort(int low, int high)
{
	int pivot;
	if (low < high)
	{
		pivot = (low + high) / 2;
		recursiveMergeSort(low, pivot);
		recursiveMergeSort(pivot + 1, high);
		merge(low, pivot + 1, high);
	}
}

void list::merge(int first, int second, int high)
{
	listEntry temp[maxlist];
	int i, last = 0;
	int pivot = second - 1;
	int low = first;
	while (first <= pivot&&second <= high)
	{
		if (data[first] <= data[second])
		{
			temp[last++] = data[first++];
		}
		else
			temp[last++] = data[second++];
	}
	if (first > pivot)
		while (second <= high)
			temp[last++] = data[second++];
	else
		while (first <= pivot)
			temp[last++] = data[first++];
	for (i = 0; i < last; i++)
		data[low++] = temp[i];
}

void list::swap(int low, int high)
{
	listEntry temp;
	temp = data[low];
	data[low] = data[high];
	data[high] = temp;
}
