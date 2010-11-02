[filename, pathname] = uigetfile({'*.bmp;*.jpg;*.png'}, 'Select file');
fn = [pathname, filename];

img = imread(fn);
img = im2bw(img, graythresh(img));
img = imcomplement(img);

CC = bwconncomp(img);
stats = regionprops(CC, 'EulerNumber');
nums = [stats.EulerNumber];

acount = 0;
bcount = 0;
ccount = 0;

for i = nums
    if i == 0
        acount = acount + 1;
    elseif i == 1
        ccount = ccount + 1;
    elseif i == -1
        bcount = bcount + 1;
    end;
end

str = sprintf('There is %u As, %u Bs and %u Cs', acount, bcount, ccount);
msgbox(str , 'Results');
