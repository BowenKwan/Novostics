#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#include "/Users/bowenpyk/Desktop/Novostics/htslib/include/htslib/*"

#include <htslib/sam.h>
#include <htslib/hts.h>

//#include <inttypes.h>

#include "utils.h"

using namespace std;


int main(int argc, char *argv[])
//int main()
{
    using std::chrono::high_resolution_clock;
    using std::chrono::duration_cast;
    using std::chrono::duration;
    using std::chrono::milliseconds;

    auto t1 = high_resolution_clock::now();
    
    auto t2 = high_resolution_clock::now();
    
    /* Getting number of milliseconds as an integer. */
    auto ms_int = duration_cast<milliseconds>(t2 - t1);

    /* Getting number of milliseconds as a double. */
    duration<double, std::milli> ms_double = t2 - t1;

    const char *inname = NULL, *tidname = NULL, *flags = NULL;
    int ret = EXIT_FAILURE;
    sam_hdr_t *in_samhdr = NULL;
    samFile *infile = NULL;

    int ret_r = 0, i = 0, j = 0;
    bam1_t *bamdata = NULL;
    uint8_t *data = NULL;
    uint32_t *cigar = NULL;

    vector<int> read_end;
    vector<int> read_length;

    vector<d10> result;

    vector<int> end_count;

    int range;

    range=40;

    vector<int> median; 
    vector<int> median_index;

    vector<int> upstream;
    vector<int> upstream_index;


    vector <d2> wave;

    int end_count_pt=16050055;

    // start position of NA12878 1mb = 9999911   
    end_count_pt=16050055;
    //end_count_pt=9999911;

    //
    int start_pos=16050055;
    //int start_pos=9999911;

    int count=0;

    int current_pos;
    int current_length;

    int count_gap100=0;
    int count_gap1000=0;
    int count_gap10000=0;
    int count_gap100000=0;
    // read_bam infile
    //if (argc != 2) {
    //    print_usage(stderr);
    //    goto end;
    //}

    //inname = "./NA12878_1mb.bam";
    //inname = "/mnt/dt2_dc2stor_data202/users/data/pykwan/Novostics/BAM_files/GM1451.chr22.srt.clipper.bam";
    //inname = "/mnt/dt2_dc2stor_data202/users/data/pykwan/Novostics/BAM_files/GM1451.chr21.srt.clipper.bam"; 
    //inname = "/mnt/dt2_dc2stor_data202/users/data/pykwan/Novostics/BAM_files/GM1451.chr20.srt.clipper.bam";
    //inname = "/mnt/hwt2_data1/wlpeng/Project_data/2018_kun_PNAS_preferred_ends/GM1451_merged_all/GM1451.chr6.srt.clipper.bam";
      
    //inname = "/mnt/hwt2_data1/wlpeng/Project_data/2018_kun_PNAS_preferred_ends/GM1451_merged_all/GM1451.chr5.srt.clipper.bam";    
    //inname = "/mnt/hwt2_data1/wlpeng/Project_data/2018_kun_PNAS_preferred_ends/GM1451_merged_all/GM1451.chr4.srt.clipper.bam";
    //inname = "/mnt/hwt2_data1/wlpeng/Project_data/2018_kun_PNAS_preferred_ends/GM1451_merged_all/GM1451.chr3.srt.clipper.bam";
    //inname = "/mnt/dt2_dc2stor_data202/users/data/pykwan/Novostics/BAM_files/GM1451.chr2.srt.clipper.bam";
    //inname = "/mnt/dt2_dc2stor_data202/users/data/pykwan/Novostics/BAM_files/GM1451.chr1.srt.clipper.bam";
    if(argc==1){
    inname = "./NA12878_1mb.bam";
    } 
    else
    {
    inname = argv[1];;
    }
    printf("BAM file name %s\n",inname);
    
    printf("Begin\n");

    if (!(bamdata = bam_init1())) {
        printf("Failed to allocate data memory!\n");
        goto end;
    }

    // open input file
    if (!(infile = sam_open(inname, "r"))) {
        printf("Could not open %s\n", inname);
        goto end;
    }
    // read header
    if (!(in_samhdr = sam_hdr_read(infile))) {
        printf("Failed to read header from file!\n");
        goto end;
    }

    printf("Start reading\n");
    
    t1 = high_resolution_clock::now();
    
    //printf("current POS: %d", current_pos);
            
    //while ((ret_r = sam_read1(infile, in_samhdr, bamdata)) >= 0)
    while ((ret_r = sam_read1(infile, in_samhdr, bamdata)) >= 0 && i<10)
    {
        //if(i%300000==0){
        //printf("%d\n",i);
	//}
	//QNAME FLAG RNAME POS MAPQ CIGAR RNEXT PNEXT TLEN SEQ QUAL [TAG:TYPE:VALUE]…
        
        //printf("NAME: %s\n", bam_get_qname(bamdata));                                   //get the query name using the macro
        flags = bam_flag2str(bamdata->core.flag);                                       //flags as string
        tidname = sam_hdr_tid2name(in_samhdr, bamdata->core.tid);
        //printf("RNAME/TID: %d - %s\n", bamdata->core.tid, tidname? tidname: "" );       //retrieves the target name using the value in bam and by referring the header
        if(j==0)
	{
	printf("POS: %" PRIhts_pos "\n", bamdata->core.pos + 0);                          //internally position is 0 based and on text output / SAM it is 1 based
	}
	//printf("current POS: %d", current_pos);
	//printf("\nTLEN/ISIZE: %" PRIhts_pos "\n", bamdata->core.isize);

        if(bamdata->core.isize>0)
        {
            read_end.push_back(bamdata->core.pos);
            read_length.push_back(bamdata->core.isize);

            // checking for extreme read length
            //if(bamdata->core.isize>1000){
            //    printf("POS: %d \n", read_end.back());                          //internally position is 0 based and on text output / SAM it is 1 based
            //    printf("\nTLEN/ISIZE: %d\n", read_length.back());
            //}
        //printf("read:\n");
        //print_vector(read_end);
        //printf("length:\n");
        //print_vector(read_length);
        
	
	 
	if((read_end.back()-current_pos)>100000 && i>0)
	{
	count_gap100000++;
	//printf("over 100000:\n i %d pos %d pos %"PRIhts_pos " gap %d\n",i,current_pos,bamdata->core.pos+0, read_end.back()-current_pos);
	printf("%d %d %"PRIhts_pos " %d\n",i,current_pos,bamdata->core.pos+0, read_end.back()-current_pos);
	}
	else if((read_end.back()-current_pos)>10000 && i>0)
	{
		count_gap10000++;
		printf("%d %d %"PRIhts_pos " %d\n",i,current_pos,bamdata->core.pos+0, read_end.back()-current_pos);        
		//printf("over 10000:\n i %d pos %d pos %"PRIhts_pos " gap %d\n",i,current_pos,bamdata->core.pos+0, read_end.back()-current_pos);
	}
		else if((bamdata->core.pos-current_pos)>1000 && i>0)
		{
			count_gap1000++;
			//printf("over 1000:\n i %d pos %d pos %"PRIhts_pos " gap %d\n",i,current_pos,bamdata->core.pos+0, read_end.back()-current_pos);

		}	
			else if((bamdata->core.pos-current_pos)>100  && i>0)
                	{
                        	count_gap100++;
				//printf("over 1000:\n i %d pos %d pos %"PRIhts_pos " gap %d\n",i,current_pos,bamdata->core.pos+0, read_end.back()-current_pos);
                	}	

		
	

        // store read end position and length to vectors
        current_pos= bamdata->core.pos;

        current_length= bamdata->core.isize;

        // collect read end property
//        count_end(result,bamdata->core.pos, bamdata->core.isize, end_count, count, end_count_pt,median,median_index,upstream,upstream_index,range, start_pos,wave);
        //printf("\n\n");
        i++;
        }
	j++;
    }

    if (ret_r == -1) {
        // no error!
        ret = EXIT_SUCCESS;

        printf("Total number of read = %d\n",i);
    }
    else {
        printf("Failed to read data\n");
    }

    // adjust for last entry

  //  end_count.push_back(count);
  //  coverage_start(wave,end_count_pt,count);
    
  //  update_vectors(result, end_count_pt, count, median, median_index, upstream, upstream_index, range, start_pos,wave);
    
    // fill up last downstream 
  //  end_wrap(result,current_pos+1,median, median_index,upstream,upstream_index, range, start_pos);
    
    t2 = high_resolution_clock::now();
    
    //print_10dvector(result);

    //print_vector(read_end);
    //print_vector(read_length);
    //print_2dvector(wave);

    printf("%ld %d %d\n",max_element(read_length.begin(),read_length.end())-read_length.begin(),read_end[max_element(read_length.begin(),read_length.end())-read_length.begin()],*max_element(read_length.begin(),read_length.end()));

    /* Getting number of milliseconds as an integer. */
    ms_int = duration_cast<milliseconds>(t2 - t1);

    /* Getting number of milliseconds as a double. */
    ms_double = t2 - t1;

    printf("%ld ms\n",ms_int.count());
    printf("%f ms\n",ms_double.count());
    printf("Total number of read = %d\n",i);

    printf("gap 100: %d\n", count_gap100);
    printf("gap 1000: %d\n", count_gap1000);
    printf("gap 10000: %d\n", count_gap10000);
    printf("gap 100000: %d\n", count_gap100000);
    //vec_to_file("chr22 read_end base", read_end);
    //vec_to_file("chr22 read_length base", read_length);
    //vec2d_to_file("chr22 wave", wave);
    //vec10d_to_file("chr22 result", result);
    
end:
    //cleanup
    if (in_samhdr) {
        sam_hdr_destroy(in_samhdr);
    }
    if (infile) {
        sam_close(infile);
    }
    if (bamdata) {
        bam_destroy1(bamdata);
    }
    return ret;
}

