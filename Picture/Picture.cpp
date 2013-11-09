#include "Picture.h"


Picture::Picture(string file="Ouran.ppm", int step=16){

	fileName=file;

	boost::iostreams::mapped_file mmap(fileName, boost::iostreams::mapped_file::readonly);
    pointer_mmap = mmap.const_data();
    mmap_length = pointer_mmap + mmap.size();
	this->step=step;

	setPicPara();
	FFillStructureWithPicInfo();

	cout << "Pixel 0, 1718: " << matrix_Red[1717] <<" ";
	cout << matrix_Green[1717]<<" ";
	cout << matrix_Blue[1717];
};

Picture::~Picture(){
};

void Picture::setPicPara(){
	int i=0;
	//skip first tow lines
    while (i<2){
       if ((pointer_mmap = static_cast<const char*>(memchr(pointer_mmap, '\n', mmap_length -pointer_mmap)))){
		i++;
		++pointer_mmap;
	    }
	}
	//const  char index= *pointer_mmap;
	/*string width, height;
	while (!isspace(*pointer_mmap) ){//lese bis zu space 
	width+=*pointer_mmap;
	pointer_mmap++;
	}
	while (isspace(*pointer_mmap)){//skip space 
	*pointer_mmap;
	pointer_mmap++;
	}


	while(!isspace(*pointer_mmap)){
	height+=*pointer_mmap;
	pointer_mmap++;
	}
	while (isspace(*pointer_mmap)){//skip space 
	*pointer_mmap;
	pointer_mmap++;
	}*/

	this->width= readSingleValue();// atoi(width.c_str());
	this->height= readSingleValue();//atoi(height.c_str());
	this->maxColorValue=readSingleValue();
	cout <<"Width: " << this->width << "Height: " << this->height<< " Color: "<< maxColorValue <<endl;
	setFrame();
}

void Picture::setFrame(){

		if((width>step) & (width % step == 0)){
			width_frame=width;
		}
		else if ((width>step) & (width % step != 0)){
			 width_frame=(width/step+1)*step;
		}
		if((height>step) & (height % step == 0)){
			height_frame=width;
		}
		else if ((height>step) & (height % step != 0)){
		height_frame=(height/step+1)*step;
		}

};

void Picture::FFillStructureWithPicInfo(){
    
	matrix_Red= valarray<int> (height_frame*width_frame); // hier kein array damit später slice möglich ist!!
	matrix_Green= valarray<int> (height_frame*width_frame);
	matrix_Blue=valarray<int> (height_frame*width_frame);
	
	for(int i=0;i<height_frame;i++){
		for(int j=0;j<width_frame;j++){
			if(j<width){
			matrix_Red[i*width_frame+j]=readSingleValue();
			matrix_Green[i*width_frame+j]=readSingleValue();
			matrix_Blue[i*width_frame+j]=readSingleValue();
			}
			else{//Farbwerte auffüllen
			matrix_Red[i*width_frame+j]=matrix_Red[i*width_frame+width-1];
			matrix_Green[i*width_frame+j]=matrix_Green[i*width_frame+width-1];
			matrix_Blue[i*width_frame+j]=matrix_Blue[i*width_frame+width-1];
			}	
		}
	}
}




int Picture::readSingleValue(){

	if(!*pointer_mmap){return 0;}

	string tmp;

	while (!isspace(*pointer_mmap) ){//lese bis zu space 
	tmp+=*pointer_mmap;
	pointer_mmap++;
	}
	while (isspace(*pointer_mmap)){//skip space 
	*pointer_mmap;
	pointer_mmap++;
	}
	return atoi(tmp.c_str());

}

void main(){

	Picture * pic= new Picture("Ouran.ppm"); 
	system("pause");
	delete pic;
}
