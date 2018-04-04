use strict;
use locale;
   
my @lignec=();
my  %cooc;
my  %coocproba;
my %freq;

#while  (my $filename=<STDIN>)

  
# {      

# my $compteur=0;
 #open(Premier,"../../../");
# my $compteur=0;
 while (my $ligne= <STDIN>){
 chomp $ligne;
 #$compteur=+1;
# if ($compteur%1000==0){print $compteur,"\n";}
    
     {$_=$ligne;
         
      /([^[^\t]*)\t([0-9]*)\t(.*)/;
	     if ((length($1)>1)&&(int($2)>30))
	 {   

my $pivot=$1;
#print $pivot, "\n";
 if (defined ($freq{$pivot}))
        { $freq{$pivot}+=int($2);}
  else {$freq{$pivot}=int($2);}
 # if ($freq{$pivot}>5)
  {#print $3, "\n";
         my @segm=split(/[,\t]+/,$3);
         for (my $i=0;$i<=$#segm;$i++)
         {#print  $segm[$i], "\n";
             
             my @fields = split( /\[/, $segm[$i]);
             #print  $fields[0], " ", $fields[1], "\n";
            if ((int($fields[1])>2)&&(length($fields[0])>1))
            {
         my $cooccurrent=$fields[0];
         if (defined $cooc{$pivot}{$cooccurrent})
         {$cooc{$pivot}{$cooccurrent}+=int($fields[1]);}
         else {$cooc{$pivot}{$cooccurrent}=int($fields[1]);}
 
         }
         else
         {if (int($fields[1])<=2) {last;}}
         }
  }
     }
     }
 }
 

 
 
 
   foreach my $pivot ( keys %cooc ) {
            my %cooc_pivot = %{ $cooc{$pivot} };
   foreach my $cooccurrent ( keys %cooc_pivot ) {
if ((defined ($freq{$pivot}))&&($freq{$pivot}>0)&&(defined ($freq{$cooccurrent}))&&($freq{$cooccurrent}>0)&&((3*$cooc{$pivot}{$cooccurrent})<$freq{$pivot})&&(($cooc{$pivot}{$cooccurrent}/$freq{$pivot})>0.01)&&(($cooc{$pivot}{$cooccurrent}/$freq{$cooccurrent})>0.001))
  {
      $coocproba{$pivot}{$cooccurrent} =$cooc{$pivot}{$cooccurrent}*10000/($freq{$pivot}*$freq{$cooccurrent});#/($freq{$pivot}) ;#
      #print $pivot, $cooccurrent, " " , $coocproba{$pivot}{$cooccurrent}, "\n";
}
else 
{
 #$coocproba{$pivot}{$cooccurrent} =0;

 #{ print $pivot, $freq{$pivot}, $cooccurrent,  $freq{$cooccurrent}, " n " , $coocproba{$pivot}{$cooccurrent}, "\n";}
}
}
}
%cooc=();
 
     
     foreach my $pivot ( sort keys %coocproba ) {
         if (length($pivot)>3)
         {print  $pivot, "\t:\t";
   my %cooc_pivot = %{ $coocproba{$pivot} };
   foreach my $cooccurrent
  
  ( sort { $cooc_pivot{$b} <=> $cooc_pivot{$a} or $a cmp $b} 
            keys %cooc_pivot ) {
                if ((length($cooccurrent)>3)&&($cooc_pivot{$cooccurrent}>0.3))
                {print  $cooccurrent, "\t";                                             }
  }
   print  "\n";
         }
 }
 
 
 
 

