#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_YEAR_DURATION	10	// 기간

// 구조체 선언
typedef struct {
	char	name[20];		// 이름
	char	sex;			// 성별 M or F
	int		freq[MAX_YEAR_DURATION]; // 연도별 빈도
} tName;

typedef struct {
	int		len;		// 배열에 저장된 이름의 수
	int		capacity;	// 배열의 용량 (배열에 저장 가능한 이름의 수)
	tName	*data;		// 이름 배열의 포인터
} tNames;

// 함수 원형 선언

// 연도별 입력 파일을 읽어 이름 정보(연도, 이름, 성별, 빈도)를 이름 구조체에 저장
// 이미 구조체에 존재하는(저장된) 이름은 해당 연도의 빈도만 저장
// 새로 등장한 이름은 구조체에 추가
// 주의사항: 동일 이름이 남/여 각각 사용될 수 있으므로, 이름과 성별을 구별해야 함
// 주의사항: 정렬 리스트(ordered list)를 유지해야 함 (qsort 함수 사용하지 않음)
// 이미 등장한 이름인지 검사하기 위해 bsearch 함수를 사용
// 새로운 이름을 저장할 메모리 공간을 확보하기 위해 memmove 함수를 이용하여 메모리에 저장된 내용을 복사
// names->capacity는 2배씩 증가
int compare( const void *n1, const void *n2);
int binary_search( const void *key, const void *base, size_t nmemb, size_t size, int (*compare)(const void *, const void *));

void load_names( FILE *fp, int start_year, tNames *names){
	
	int year;
	int end;
	tName newName;
	int newcnt, year_index, found_index;
	tName* index;
	end = fscanf(fp, "%d\t%s\t%c\t%d\n", &year, newName.name, &(newName.sex), &newcnt);

	while(end != -1){
		
		year_index = year - start_year;
		
		//이미 있는 이름
		if(bsearch(&newName, names->data, names->len, sizeof(tName), compare) != NULL){
			index = bsearch(&newName, names->data, names->len, sizeof(tName), compare);
			index->freq[year_index] = newcnt;
			
			end = fscanf(fp, "%d\t%s\t%c\t%d\n", &year, newName.name, &(newName.sex), &newcnt);
		}
		
		//새 이름

		else{
			
			found_index = binary_search(&newName, names->data, names->len, sizeof(tName), compare);
			
			(names->len) += 1;
			if(names->len >= names->capacity){
				(names->capacity) *= 2;
				names->data = (tName *)realloc(names->data, names->capacity * sizeof(tName));
			}
			
			memmove(((names->data) + found_index) + 1, (names->data) + found_index, ((names->len) - found_index - 1) * sizeof(tName));
			memset(((names->data) + found_index)->freq, 0, MAX_YEAR_DURATION * sizeof(int));
			
			strcpy(((names->data) + found_index)->name, newName.name);	
			((names->data) + found_index)->sex = newName.sex;
			((names->data) + found_index)->freq[year_index] = newcnt;
			
			end = fscanf(fp, "%d\t%s\t%c\t%d\n", &year, newName.name, &(newName.sex), &newcnt);
		}
	}
}

// 구조체 배열을 화면에 출력
void print_names( tNames *names, int num_year){
	int i = 0;
	int j;
	while(names->len > 0){
		printf("%s\t%c",((names->data) + i)->name, ((names->data) + i)->sex);
		
		for(j=0; j < num_year; j++){
			printf("\t%d",((names->data) + i)->freq[j]);
		}
		
		printf("\n");
		i++;
		(names->len)--;
	}
}

// bsearch를 위한 비교 함수
int compare( const void *n1, const void *n2){
	
	tName* name1 = (tName*)n1;
	tName* name2 = (tName*)n2;
	
	if(strcmp(name1->name, name2->name) < 0)
		return -1;
	
	else if(strcmp(name1->name, name2->name) > 0)
		return 1;
	
	else if(strcmp(name1->name, name2->name) == 0){
	
		if(name1->sex > name2->sex)
			return 1;
		
		else if(name1->sex < name2->sex)
			return -1;
		
		else
			return 0;
	}
}

// 이진탐색 함수
// return value: key가 발견되는 경우, 배열의 인덱스
//				key가 발견되지 않는 경우, key가 삽입되어야 할 배열의 인덱스
int binary_search( const void *key, const void *base, size_t nmemb, size_t size, int (*compare)(const void *, const void *)){
	
	int left = 0, right = nmemb - 1;
	int mid;
		
	while(left <= right){
			
		mid =  (left + right) / 2;
			
		if(compare(base + mid * size, key) > 0){
			right = mid - 1;
		}
			
		else if(compare(base + mid * size, key) < 0){
			left = mid + 1;
		}
			
		else if(compare(base + mid * size, key) == 0){
			return mid; 
		}
	}

	return left;
}
// 함수 정의

// 이름 구조체 초기화
// len를 0으로, capacity를 1로 초기화
// return : 구조체 포인터
tNames *create_names(void)
{
	tNames *pnames = (tNames *)malloc( sizeof(tNames));
	
	pnames->len = 0;
	pnames->capacity = 1;
	pnames->data = (tName *)malloc(pnames->capacity * sizeof(tName));

	return pnames;
}

// 이름 구조체에 할당된 메모리를 해제
void destroy_names(tNames *pnames)
{
	free(pnames->data);
	pnames->len = 0;
	pnames->capacity = 0;

	free(pnames);
}
	
////////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
	tNames *names;
	FILE *fp;
	
	if (argc != 2)
	{
		fprintf( stderr, "Usage: %s FILE\n\n", argv[0]);
		return 1;
	}

	// 이름 구조체 초기화
	names = create_names();
	
	fp = fopen( argv[1], "r");
	if (!fp)
	{
		fprintf( stderr, "cannot open file : %s\n", argv[1]);
		return 1;
	}

	fprintf( stderr, "Processing [%s]..\n", argv[1]);
		
	// 연도별 입력 파일(이름 정보)을 구조체에 저장
	load_names( fp, 2009, names);
	
	fclose( fp);
	
	// 이름 구조체를 화면에 출력
	print_names( names, MAX_YEAR_DURATION);

	// 이름 구조체 해제
	destroy_names( names);
	
	return 0;
}










