from Crypto.Cipher import AES

mode = AES.MODE_CBC
# key =  b'\x00\x11\x22\x33\x44\x55\x66\x77\x88\x99\xaa\xbb\xcc\xdd\xee\xff'
# msg =  b'\x5f\x3f\x1c\x74\x4e\xe6\xc9\x12\xe4\x24\xe3\x0d\xd8\x06\x92\x4a'
# iv  =  b'\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00'
# cip =  b"\x36\x48\xb4\x4b\x70\x3b\x7d\x35\xd4\xb2\x1b\x7a\xd9\xb7\xd0\xf4"


#c = b'\x67\xf1\xe5\x98\x2b\x96\x4e\xd8\x28\xc6\xa9\x62\xb1\x68\x3e\xe8\xbd\xdd\x2c\x57\xaa\x65\xa8\xae\x9d\x07\x50\xd6\x3a\x27\x7f\x56'
k = b'\x36\x48\xb4\x4b\x70\x3b\x7d\x35\xd4\xb2\x1b\x7a\xd9\xb7\xd0\xf4\x5f\x3f\x1c\x74\x4e\xe6\xc9\x12\xe4\x24\xe3\x0d\xd8\x06\x92\x4a'
i = b'\x06\x28\x62\x69\x0f\x3c\x2f\x2f\xef\x35\xec\xff\xaf\x0a\x22\x59'
m = b'THUCTF{Y0u_cAn_s3nd_4nd_sNiff3r_802.11_r4w_pAckag3}'.ljust(64,b"\x00")

cryptos = AES.new(k, mode, i)
cc = cryptos.encrypt(m)
#msg = cryptos.decrypt(c)
ccc= ""
b = cc.hex()
for i in range(0,len(b),2):
    	ccc += '\\x'+b[i]+b[i+1]
payload = ccc


print(cc.hex())
print(len(cc))
print(payload)

# if(cipher_text == cip):
#     print("yes")

# print(cipher_text.hex())


