//
//  OADDevice.h
//  BleArduino
//
//  Created by Raymond Kampmeier on 8/16/13.
//  Copyright (c) 2013 Punch Through Design. All rights reserved.
//

#import <Foundation/Foundation.h>

#ifdef __IPHONE_OS_VERSION_MIN_REQUIRED
#import <CoreBluetooth/CoreBluetooth.h>
#else
#import <IOBluetooth/IOBluetooth.h>
#endif

#import "DevInfoProfile.h"
#import "Profile_Protocol.h"

#define HI_UINT16(a) (((a) >> 8) & 0xff)
#define LO_UINT16(a) ((a) & 0xff)

#define SERVICE_OAD                     @"0xF000FFC0-0451-4000-B000-000000000000"
#define CHARACTERISTIC_OAD_NOTIFY       @"0xF000FFC1-0451-4000-B000-000000000000"
#define CHARACTERISTIC_OAD_BLOCK        @"0xF000FFC2-0451-4000-B000-000000000000"

@protocol OAD_Delegate;
@protocol OadDeviceDelegate;

@interface OadProfile : NSObject <Profile_Protocol>

@property (nonatomic, assign) id<OAD_Delegate, ProfileDelegate_Protocol> delegate;

@property (strong,nonatomic) NSData *imageFile;
@property int nBlocks;
@property int nBytes;
@property int iBlocks;
@property int iBytes;
@property BOOL canceled;
@property BOOL inProgramming;
@property (nonatomic,strong) NSTimer *imageDetectTimer;
@property uint16_t imgVersion;

-(id)initWithPeripheral:(CBPeripheral*)aPeripheral delegate:(id<OAD_Delegate, ProfileDelegate_Protocol>)delegate;

// Returns FALSE if OAD is not supported on the device. TRUE otherwise
// See callback method: -(void)FirmwareVersion:(NSString*)version isNewer:(BOOL)isNewer;
-(BOOL)checkForNewFirmware:(NSString*)newFirmwareVersion;

// Returns false if OAD is not supported on the device. Returns true if OAD is supported. Returns false when the breathalyzer is not connected.
// Parameters imageApath and imageBpath are full paths to the images .bin files
// See callback methods:
//-(void)oadDeviceFailedOADUpload:(OAD_BLEDevice*)oadDevice;
//-(void)oadDeviceCompletedOADUpload:(OAD_BLEDevice*)oadDevice;
//-(void)oadDevice:(OAD_BLEDevice*)oadDevice OADUploadTimeLeft:(NSNumber*)seconds withPercentage:(NSNumber*)percentageComplete;
//-(void)oadDeviceOADInvalidImage:(OAD_BLEDevice*)oadDevice;
-(BOOL)updateFirmwareWithImageAPath:(NSString*)imageApath andImageBPath:(NSString*)imageBpath;

// Cancels firmware update
// No callbacks needed
-(void)cancelUpdateFirmware;

@end



@protocol OAD_Delegate <NSObject>

@optional
// Callback for method: -(BOOL)checkForNewFirmware:(NSString*)newFirmwareVersion
// Returns TRUE if the firmware is newer than on the device. FALSE otherwise
-(void)device:(OadProfile*)device firmwareVersion:(NSString*)version isNewer:(BOOL)isNewer;

// Callback for method: -(BOOL)updateFirmwareWithImageAPath:(NSString*)imageApath andImageBPath:(NSString*)imageBpath;
// Indicates when the uploading of firmware fails.
// Called when not connected to breathalyzer.
// This happens when it sends the writeWithoutResponse packets too fast and packets are dropped
// Solution: Have user try again. Or slow down transfer time
-(void)deviceFailedOADUpload:(OadProfile*)device;

// Callback for method: -(BOOL)updateFirmwareWithImageAPath:(NSString*)imageApath andImageBPath:(NSString*)imageBpath;
// Called when firmware upload is complete
-(void)deviceCompletedOADUpload:(OadProfile*)device;

// Callback for method: -(BOOL)updateFirmwareWithImageAPath:(NSString*)imageApath andImageBPath:(NSString*)imageBpath;
// Called every time the time left changes
-(void)device:(OadProfile*)device OADUploadTimeLeft:(NSNumber*)seconds withPercentage:(NSNumber*)percentageComplete;

// Callback for method: -(BOOL)updateFirmwareWithImageAPath:(NSString*)imageApath andImageBPath:(NSString*)imageBpath;
// Called when firmware images are invalid
// Solution: Use correct images
-(void)deviceOADInvalidImage:(OadProfile*)device;

@end

