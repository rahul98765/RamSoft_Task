// DicomRead.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <windows.h>
#include <iostream>

#include "dcmtk/config/osconfig.h"
#include "dcmtk/dcmdata/dcfilefo.h"
#include "dcmtk/dcmdata/libi2d/i2d.h"
#include "dcmtk/dcmdata/libi2d/i2djpgs.h"
#include "dcmtk/dcmdata/libi2d/i2dplsc.h"
#include "dcmtk/dcmdata/dctk.h"
#include "dcmtk/dcmimgle/dcmimage.h"

using namespace std;

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " <path_to_image>" << std::endl;
        return -1;
    }
    try
    {
        DcmFileFormat fileRead;
        OFCondition status = fileRead.loadFile(argv[1]);
        if (status.good())
        {
            OFString patient_Name, modality;
            Uint16 ImgHeight, ImgWidth;
            DcmElement* pixelement = NULL;
            fileRead.getDataset()->findAndGetOFString(DCM_PatientName, patient_Name, true);
            fileRead.getDataset()->findAndGetOFString(DCM_Modality, modality, true);
            fileRead.getDataset()->findAndGetUint16(DCM_Rows, ImgHeight);
            fileRead.getDataset()->findAndGetUint16(DCM_Columns, ImgWidth);
            
            DicomImage* image = new DicomImage(argv[1]);
            const DiPixel* pix = (image->getInterData());
            EP_Representation rep = pix->getRepresentation();
            if (rep == EPR_Uint16) {
                Uint16* pixelData = (Uint16*)pix->getData();
                FILE* file = fopen("output.jpeg", "wb");
                fwrite(pixelData, sizeof(Uint16*), sizeof(pixelData), file);
            }
                       
            cout << "Patient's Name: " << patient_Name << endl;
            cout << "Modality: " << modality << endl;
            cout << "Image Height: " << ImgHeight << endl;
            cout << "Image Width: " << ImgWidth << endl;
        }
        else
        {
            cout << "Error: Not a valid DICOM file (" << status.text() << ")" << endl;
            return -1;
        }
    }
    catch (std::exception ex)
    {
        std::cout << "exception in reading dicom file:" << ex.what() << std::endl;
        return -1;
    }
}
