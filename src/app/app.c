#include <stdio.h>
#include <stdlib.h>
#include <libusb-1.0/libusb.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>


// CUSTOM
#include "app/app.hh"

#define PRIVATE static
#define PUBLIC

//
// PRIVATE VARIABLES
//

static libusb_device_handle* dev = NULL;

static int __loop_period_seconds = 1;

static uint16_t __target_device_vendor_id  = 0x0fe7;
static uint16_t __target_device_product_id = 0x4001;



//
// PRIVATE FUNCTIONS
//

PRIVATE int find_and_open_device(void)
{
    dev = libusb_open_device_with_vid_pid(NULL,__target_device_vendor_id, __target_device_product_id);
    if(!dev) {
        errno = ENODEV;
        return -1;
    }

    return 0;
}


PRIVATE void dump_buffer(const unsigned char* buf, int size)
{
    printf("BUFFER: [ ");
    for(int i = 0; i < size; ++i)
        printf("%c ", buf[i]);

    printf("]\n");
}

PRIVATE int loop(void)
{
    int status = 0;
    int try_counter = 0;
    int max_try = 5;
    while(try_counter < max_try) {

        sleep(__loop_period_seconds);
        if(try_counter == max_try) {
            printf( "Maximum try done.\n");
            break;
        }

        ++try_counter;

        status = find_and_open_device();
        if(status < 0) {
            fprintf(stderr, "mitutoyo:: could not find/open device\n");
            continue;
        }

        if(libusb_kernel_driver_active(dev, 0))
            libusb_detach_kernel_driver(dev, 0);

        
        status = libusb_reset_device(dev);
        if(status) {
            fprintf(stderr, "mitutoyo:: reset device error %d - %s\n", status, libusb_strerror(status));
            libusb_close(dev);
            continue;
        }

        status = libusb_set_configuration(dev, 1);
        if(status) {
            fprintf(stderr, "mitutoyo:: set configuration error %d - %s\n", status, libusb_strerror(status));
            libusb_close(dev);
            continue;
        }

        status = libusb_claim_interface(dev, 0);
        if(status < 0) {
            fprintf(stderr, "mitutoyo:: error while caliming interface %d - %s\n", status, libusb_strerror(status));
            libusb_close(dev);
            continue;
        }


        uint8_t bmRequestType;
        uint8_t bRequest;
        uint16_t wValue;
        uint16_t wIndex;
        unsigned char data[64] = {};


        bmRequestType = 0x40;
        bRequest = 0x01;
        wValue = 0xA5A5;
        wIndex = 0;

        status = libusb_control_transfer(dev, 0x40, 0x01, 0xA5A5, 0, NULL, 0, 1000);
        if(status < 0) {
            fprintf(stderr, "mitutoyo:: control transfer error %d - %s\n", status, libusb_strerror(status));
            libusb_close(dev);
            continue;
        }



        bmRequestType = 0xC0;
        bRequest = 0x02;
        wValue = 0;
        wIndex = 0;

        status = libusb_control_transfer(dev, bmRequestType, bRequest, wValue, wIndex, data, 1, 1000);
        if(status < 0) {
            fprintf(stderr, "mitutoyo:: control transfer error %d - %s\n", status, libusb_strerror(status));
            libusb_close(dev);
            continue;
        }


        memset(data, 0, sizeof(data));

        bmRequestType = 0x40;
        bRequest = 0x03;
        wValue = 0;
        wIndex = 0;

        data[0] = '1'; 
        data[1] = '\r';
        
        status = libusb_control_transfer(dev, bmRequestType, bRequest, wValue, wIndex, data, 2, 1000);
        if(status < 0) {
            fprintf(stderr, "mitutoyo:: control transfer error %d - %s\n", status, libusb_strerror(status));
            libusb_close(dev);
            continue;
        }

        while(1) {
            int transferred = 0;

            memset(data, 0, sizeof(data));
            status = libusb_bulk_transfer(dev, 0x81, data, 13, &transferred, 3000);
            
            if(status < 0) {
                fprintf(stderr, "mitutoyo:: bulk transfer error %d - %s\n", status, libusb_strerror(status));
                break;
            }

            dump_buffer(data + 3, 10);
        }

        libusb_release_interface(dev, 0);
        libusb_close(dev);
    }

    return 0;
}


PUBLIC int appMain(void)
{
    int status;

    status = libusb_init(NULL);
    if(status != LIBUSB_SUCCESS) {
        fprintf(stderr, "Failed to initialize libusb.\n");
        return -status;
    }

    return loop();
}