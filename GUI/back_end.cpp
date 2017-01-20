#include "back_end.hpp"



typedef Eigen::MatrixXd Matrix;
typedef Eigen::VectorXd Vector;

typedef std::pair< std::vector< std::vector<double> >, std::vector< std::vector<uint32_t> > > VerticesFaces;
typedef std::pair<std::vector<Matrix>, std::vector<Matrix> > EdgeMapProjection;
typedef std::vector<std::pair<float,float>> TextureCoords;


/**
 * @brief ShotToEigen
 * Convert a camera chot (vcg) to a matrix
 */
void ShotToEigen(const vcg::Shotf &camera,
                 Matrix &P,
                 const double aspectratio=1);

void AppendVerticesToMesh(const VerticesFaces &vf, CMesh& mesh, TextureCoords textureCoords );
std::vector<uint32_t> FindPointsFromCombinedProbability(const EdgeMapProjection& EP, const Matrix& X);

double normgarr[250]={0.7978845608028654,0.7978588234214670,0.7977816162584324,0.7976529542556376,0.7974728623108541,0.7972413752697191,0.7969585379144984,0.7966244049496487,0.7962390409841829,0.7958025205108490,0.7953149278821312,0.7947763572830835,0.7941869127010085,0.7935467078919952,0.7928558663443303,0.7921145212388002,0.7913228154059012,0.7904809012799767,0.7895889408503033,0.7886471056091459,0.7876555764968072,0.7866145438436944,0.7855242073094308,0.7843847758190374,0.7831964674962154,0.7819595095937579,0.7806741384211222,0.7793405992691956,0.7779591463322870,0.7765300426273809,0.7750535599106869,0.7735299785915258,0.7719595876435856,0.7703426845135914,0.7686795750274261,0.7669705732937454,0.7652160016051287,0.7634161903368097,0.7615714778430320,0.7596822103510724,0.7577487418529832,0.7557714339950957,0.7537506559653366,0.7516867843784055,0.7495802031588628,0.7474313034221792,0.7452404833537999,0.7430081480862715,0.7407347095744908,0.7384205864691239,0.7360662039882521,0.7336719937873004,0.7312383938273033,0.7287658482415631,0.7262548072007601,0.7237057267765694,0.7211190688038432,0.7184953007414173,0.7158348955315986,0.7131383314583957,0.7104060920045472,0.7076386657074132,0.7048365460137850,0.7020002311336758,0.6991302238931530,0.6962270315862701,0.6932911658261629,0.6903231423953685,0.6873234810954271,0.6842927055958317,0.6812313432823821,0.6781399251050083,0.6750189854251214,0.6718690618625550,0.6686906951421578,0.6654844289400974,0.6622508097299361,0.6589903866285408,0.6557037112418852,0.6523913375108052,0.6490538215567683,0.6456917215277141,0.6423055974440280,0.6388960110447045,0.6354635256337589,0.6320087059269476,0.6285321178988509,0.6250343286303787,0.6215159061567532,0.6179774193160270,0.6144194375981885,0.6108425309949154,0.6072472698500230,0.6036342247106685,0.6000039661793588,0.5963570647668167,0.5926940907457574,0.5890156140056247,0.5853222039083389,0.5816144291451040,0.5778928575943245,0.5741580561806793,0.5704105907353982,0.5666510258577897,0.5628799247780640,0.5590978492214951,0.5553053592739672,0.5515030132489470,0.5476913675559233,0.5438709765703563,0.5400423925051743,0.5362061652838579,0.5323628424151509,0.5285129688694304,0.5246570869567786,0.5207957362067833,0.5169294532501090,0.5130587717018644,0.5091842220468028,0.5053063315263840,0.5014256240277274,0.4975426199744847,0.4936578362196613,0.4897717859404104,0.4858849785348272,0.4819979195207646,0.4781111104366974,0.4742250487446539,0.4703402277352360,0.4664571364347500,0.4625762595144632,0.4586980772020071,0.4548230651949416,0.4509516945764961,0.4470844317335023,0.4432217382765306,0.4393640709622440,0.4355118816179774,0.4316656170685562,0.4278257190653585,0.4239926242176335,0.4201667639260765,0.4163485643186720,0.4125384461888063,0.4087368249356524,0.4049441105068320,0.4011607073433525,0.3973870143268233,0.3936234247289463,0.3898703261632830,0.3861281005392911,0.3823971240186315,0.3786777669737358,0.3749703939486329,0.3712753636220260,0.3675930287726116,0.3639237362466343,0.3602678269276650,0.3566256357085947,0.3529974914658304,0.3493837170356824,0.3457846291929301,0.3422005386315498,0.3386317499475939,0.3350785616242025,0.3315412660187325,0.3280201493519873,0.3245154916995282,0.3210275669850499,0.3175566429758005,0.3141029812800245,0.3106668373464114,0.3072484604655242,0.3038480937731888,0.3004659742558208,0.2971023327576653,0.2937573939899270,0.2904313765417656,0.2871244928931316,0.2838369494294154,0.2805689464578867,0.2773206782258934,0.2740923329407969,0.2708840927916126,0.2676961339723303,0.2645286267068844,0.2613817352757446,0.2582556180441006,0.2551504274916067,0.2520663102436607,0.2490034071041829,0.2459618530898684,0.2429417774658775,0.2399433037829368,0.2369665499158172,0.2340116281031576,0.2310786449886033,0.2281677016632246,0.2252788937091872,0.2224123112446374,0.2195680389697734,0.2167461562140671,0.2139467369846060,0.2111698500155200,0.2084155588184618,0.2056839217341078,0.2029749919846446,0.2002888177272113,0.1976254421082607,0.1949849033188105,0.1923672346505470,0.1897724645527532,0.1872006166900232,0.1846517100007364,0.1821257587562509,0.1796227726207924,0.1771427567119984,0.1746857116620901,0.1722516336796389,0.1698405146118935,0.1674523420076396,0.1650870991805569,0.1627447652730445,0.1604253153204826,0.1581287203158998,0.1558549472750154,0.1536039593016265,0.1513757156533100,0.1491701718074087,0.1469872795272734,0.1448269869287310,0.1426892385467504,0.1405739754022766,0.1384811350692060,0.1364106517414742,0.1343624563002302,0.1323364763810679,0.1303326364412898,0.1283508578271762,0.1263910588412335,0.1244531548093968,0.1225370581481610,0.1206426784316164,0.1187699224583635,0.1169186943182847,0.1150888954591477,0.1132804247530192,0.1114931785624652,0.1097270508065161,0.1079819330263761};

// computes an eigen matrix given a vcg shot
void ShotToEigen(const vcg::Shotf &camera,
                 Matrix &P,
                 const double aspectratio)
{
    Matrix K(3,3), R(4,4);
    P.setZero(3,4);
    K.setZero();

    camera.Extrinsics.Rot().ToEigenMatrix(R);
    R.conservativeResize(3,3);

    Matrix t(3,1);
    for(uint32_t k=0;k<3;k++)
    {
        t(k) = -camera.Extrinsics.Tra().V()[k];
    }

    float c1 = camera.Intrinsics.FocalMm / camera.Intrinsics.PixelSizeMm.X();
    float c2 = camera.Intrinsics.FocalMm / camera.Intrinsics.PixelSizeMm.Y();

    K(0,0) = -c1;
    K(1,1) = c2;
    K(2,2) = 1;
    K(0,2) = camera.Intrinsics.CenterPx.X();
    K(1,2) = camera.Intrinsics.CenterPx.Y();

    P.block<3,3>(0,0) = K*R;
    P.block<3,1>(0,3) = K*R*t;
    for(uint32_t i=0;i<4;i++)
    {
        P(2,i)=P(2,i)*aspectratio;
    }
}

// computes the edge points given a set of sketches, their rasters and the point cloud
void SketchesToEdge(const QList<Sketch *> &sketches,
                    const QMap<rasterID, Raster *> &rasters,
                    const Mesh *point_cloud,
                    QList<vcg::Point3f> &edge_points,
                    const uint32_t NumberOfNewSamples,
                    const double aspectratio)
{
    Matrix X(3,point_cloud->mesh.VN());
    uint32_t k=0;
    for(auto it =point_cloud->mesh.vert.begin(); it != point_cloud->mesh.vert.end(); ++it)
    {
        X(0,k) = it->P()[0];
        X(1,k) = it->P()[1];
        X(2,k) = it->P()[2];
        ++k;
    }

    std::vector<Matrix> Ps(sketches.size(), Matrix(3,4));
    std::vector<Matrix> Edge(sketches.size());
    auto P = Ps.begin();
    auto e = Edge.begin();
    uint32_t x,y,j,h,w,indx=0;
    uint32_t radius, absolutevalue;

    //
    // @todo: The computed edge maps can be used here
    // They only need to be computed when we are drawing on an
    // image.
    //
    DataStore *dat = DataStore::data();
    (void) dat;

    double *probdata;
    double probsum,cprob;

    std::for_each(sketches.begin(), sketches.end(), [&](Sketch * sketch)
    {
        ShotToEigen(*rasters[sketch->raster]->camera, *(P++), aspectratio);
        h = rasters[sketch->raster]->image.height();
        w = rasters[sketch->raster]->image.width();
        *e = Matrix(h,w);
        e->setZero();
        probdata = e->data();
        probsum=0;
        for(const auto& sp : sketch->points)
        {
            x = static_cast<uint32_t>(std::floorf(sp.y*h));
            y = static_cast<uint32_t>(std::floorf(sp.x*w));
            radius = static_cast<uint32_t>(sp.sizePX()/2.0);
            for(k=0;k<2*radius;++k)
            for(j=0;j<2*radius;++j)
            {
                absolutevalue = (j-radius)*(j-radius) + (k-radius)*(k-radius);
                if(x + k - radius < h && y + j - radius < w && absolutevalue < radius*radius )
                {
                    cprob = normgarr[(static_cast<uint32_t>(std::sqrt( absolutevalue ))*249) / radius ];
                    probsum += cprob;
                    probdata[x + k - radius + h*(y + j - radius)] += cprob;
                }
            }
        }
        *e /= probsum;
        /*std::stringstream ss;
        ss << "Edge" << indx++;
        Cu::print_matrix_octave(*e, ss.str());*/
        ++e;
    });

    edge_points.clear();

    auto EP = std::make_pair(Edge, Ps);
    auto indexes = FindPointsFromCombinedProbability(EP, X);
    vcg::Point3f pt;
    for(auto i : indexes)
    {
        edge_points.push_back( vcg::Point3f(X(0,i), X(1,i), X(2,i)) );
    }

    if(NumberOfNewSamples > 0)
    {
        Core::Generator sg;
        auto np = sg.Sample3DEdge(Edge, Ps, NumberOfNewSamples);
        for(const auto& n : np)
        {
            edge_points.push_back( vcg::Point3f(n[0], n[1], n[2]) );
        }
    }
}

void AppendVerticesToMesh(const VerticesFaces &vf, CMesh& mesh, TextureCoords textureCoordinates)
{
    CMesh::VertexIterator vi = vcg::tri::Allocator<CMesh>::AddVertices(mesh,vf.first.size());
    CMesh::FaceIterator fi = vcg::tri::Allocator<CMesh>::AddFaces(mesh,vf.second.size());

    std::vector<CMesh::VertexPointer> ivp(vf.first.size());
    auto tit=textureCoordinates.begin();
    for(uint32_t k=0; k<vf.first.size(); ++k)
    {
        ivp[k] = &*vi;
        vi->P()[0] = vf.first[k][0];
        vi->P()[1] = vf.first[k][1];
        vi->P()[2] = vf.first[k][2];

        if(k<textureCoordinates.size())
        {
            vi->T().u() = tit->first;
            vi->T().v() = tit->second;
            ++tit;
        }

        ++vi;
    }

    for(uint32_t k=0; k<vf.second.size(); ++k)
    {
        fi->V(0) = ivp[static_cast<int32_t>(vf.second[k][0])];
        fi->V(1) = ivp[static_cast<int32_t>(vf.second[k][1])];
        fi->V(2) = ivp[static_cast<int32_t>(vf.second[k][2])];
        ++fi;
    }
    vcg::tri::UpdateNormal<CMesh>::PerVertexNelsonMaxWeighted(mesh);
    vcg::tri::UpdateNormal<CMesh>::PerFace(mesh);
    vcg::tri::UpdateNormal<CMesh>::PerVertexNormalizedPerFace(mesh);
    vcg::tri::UpdateNormal<CMesh>::PerVertexFromCurrentFaceNormal(mesh);
}

// computes a CMesh with the surface sampling given a list of edges and a set of parameters.
// IMPORTANT: params must be cast to a specific kind of surface
void EdgesToSurface(const QList<Edge *> &edges,
                    const SurfaceInputParams *params,
                    CMesh &surface_out)
{
    if(params->SurfaceType == NONE)
    {
        return;
    }

    const uint32_t N = edges.size();
    std::vector<uint32_t> Sizes(N,0);
    uint32_t sumsizes=0;
    uint32_t k=0;
    for(const auto& e : edges)
    {
        Sizes[k] = e->points.size();
        sumsizes += Sizes[k];
        ++k;
    }

    std::vector<double> vdat( sumsizes*N*3, 0.0 );

    auto ind = vdat.begin();
    for(const auto& e : edges)
    for(const auto& p : e->points)
    for(const auto& px : p)
            *(ind++) = px;

    const double* data = vdat.data();

    VerticesFaces vf;
    TextureCoords texs(0);

    switch(params->SurfaceType)
    {
        case QUADRIC :
        case CYLINDER :
        {
            const QuadricInputParams* inputparams = static_cast<const QuadricInputParams*>(params);
            Core::Quadric quad(data, sumsizes, inputparams->sigma);
            //Cu::print_matrix_octave(data, 3, sumsizes, "AllData");

            if(params->SurfaceType == QUADRIC)
            {
                vf = quad.GetMeshRep( inputparams->SamplingDensity, data, sumsizes*N );
                //ShowVertices = quad.GetPointCloudBounds(inputparams->SamplingDensity, data, sumsizes*N  );
                quad.GetBoundedMeshRep(vf, data, sumsizes*N);
            }
            else
            {
                vf = quad.GetPrincipleCylinder(inputparams->SamplingDensity, data, sumsizes*N);
                quad.GetBoundedMeshRep(vf, data, sumsizes*N);
                //std::cerr << "error: this funcion has not been implemented" << std::endl;
            }

            if(vf.first.size() ==0 )
            {
                LOG_INFO() << "The quadric has no vertices. This means that the data is significantly different from an ellipsoid or cylinder, and so cannot be parameterised properly. Try decreasing sigma.";
            }
        } break;

        case GTM_INTERPOLATED :
        case GTM_ORTHOGONAL :
        {
            std::vector< Matrix > pts(N);

            const GTMInputParams* inputparams =  static_cast<const GTMInputParams*>(params);

            for( uint32_t k=0; k<N; ++k )
            {
                double *dmat = const_cast<double*>(data)+((k>0)?Sizes[k-1]*3:0);

                Core::Input in{dmat, 3, Sizes[k], inputparams->K, inputparams->MaxIter, inputparams->lambda, inputparams->Degree};
                Core::Output out=Core::Polynomial(in);

                // It might be worth trimming points with a low value of pi here.
                pts[k] = out.W * out.Phi;

                /*std::stringstream ss;ss << "W" << k;
                Cu::print_matrix_octave(out.W, ss.str());
                ss.clear(); ss << "Phi" << k;
                Cu::print_matrix_octave(out.Phi, ss.str());*/
            }

            double a=0.0,b=0.0;
            for(uint32_t k=0;k<3;++k)
            {
                a += (pts[0](k,0) - pts[1](k,0)) *
                     (pts[0](k,0) - pts[1](k,0));
                b += (pts[0](k,0) - pts[1](k,inputparams->K-1))*
                     (pts[0](k,0) - pts[1](k,inputparams->K-1));
            }

            if( a > b )
            {
                Cu::Matrix r =  pts[1].rowwise().reverse();
                pts[1] = r; // Bugfix. operator= copies while computing
            }

            switch(params->SurfaceType)
            {
                case GTM_ORTHOGONAL:
                {
                    vf.first = std::vector< std::vector<double> >(inputparams->K*inputparams->K, std::vector<double>(3,0.0));
                    texs = TextureCoords (inputparams->K*inputparams->K, std::make_pair(0,0));
                    for( uint32_t i=0,j,k=0; k<inputparams->K; ++k )
                    {
                        for(j=0; j<inputparams->K; ++j)
                        {
                            vf.first[i][0] = pts[0](0, k) + (pts[1](0, j) - pts[1](0, 0));
                            vf.first[i][1] = pts[0](1, k) + (pts[1](1, j) - pts[1](1, 0));
                            vf.first[i][2] = pts[0](2, k) + (pts[1](2, j) - pts[1](2, 0));
                            texs[i].first=(static_cast<float>(k)/inputparams->K);
                            texs[i].second=(static_cast<float>(j)/inputparams->K);
                            ++i;
                        }
                    }
                } break;

                case GTM_INTERPOLATED:
                {
                    vf.first = std::vector< std::vector<double> >(inputparams->K*inputparams->K, std::vector<double>(3,0.0));
                    texs = TextureCoords(inputparams->K*inputparams->K, std::make_pair(0,0));
                    double alpha=0.0;
                    for( uint32_t i=0,j,k=0; k<inputparams->K; ++k )
                    {
                        alpha = static_cast<float>(k) /inputparams->K;
                        for(j=0; j<inputparams->K; ++j)
                        {
                            vf.first[i][0] = alpha*pts[0](0, j) + (1.0-alpha)*pts[1](0, j);
                            vf.first[i][1] = alpha*pts[0](1, j) + (1.0-alpha)*pts[1](1, j);
                            vf.first[i][2] = alpha*pts[0](2, j) + (1.0-alpha)*pts[1](2, j);
                            texs[i].first=(static_cast<float>(k)/inputparams->K);
                            texs[i].second=(static_cast<float>(j)/inputparams->K);
                            ++i;
                        }
                    }
                } break;
            }

            vf.second = std::vector< std::vector<uint32_t> >(
                        (inputparams->K-1)*(inputparams->K-1)*2, std::vector<uint32_t>(3,0));

            uint32_t index=0;
            for( uint32_t j,k=0; k<inputparams->K-1; ++k )
            {
                for(j=0; j<inputparams->K-1; ++j)
                {
                    vf.second[index][0] = k + inputparams->K*j;
                    vf.second[index][1] = k + inputparams->K*j + 1;
                    vf.second[index][2] = k + inputparams->K*(j+1);
                    ++index;

                    vf.second[index][0] = k + inputparams->K*j + 1;
                    vf.second[index][1] = k + inputparams->K*(j+1) + 1;
                    vf.second[index][2] = k + inputparams->K*(j+1);
                    ++index;
                }
            }
        } break;

        default :
        {

        }break;
    }

    AppendVerticesToMesh(vf, surface_out, texs);

    switch(params->SurfaceType)
    { // Invert normals if necessary
        case GTM_ORTHOGONAL:
        case GTM_INTERPOLATED:
        {
            DataStore *dat = DataStore::data();
            vcg::Point3f vd = dat->rasters[dat->current_raster]->camera->GetViewDir();
            CMesh::VertexIterator vi = surface_out.vert.begin();
            auto norm = vi->N();
            double dt = vd.X()*norm[0] + vd.Y()*norm[1] + vd.Z()*norm[2];
            if(dt<0)
            {
                vcg::tri::Clean<CMesh>::FlipMesh(surface_out);
            }
        } break;
    }
}

std::vector<uint32_t> FindPointsFromCombinedProbability(const EdgeMapProjection& EP, const Matrix& X)
{
    auto iP = EP.second.begin();
    int32_t index =0;
    const uint32_t EPN = EP.first.size();
    const uint32_t XC = X.cols();
    Matrix probs(EPN, XC);
    probs.setZero();

    Matrix mm(EPN, 1),mul;
    mm.setZero();

    Matrix XHomog(4,XC);
    XHomog.setOnes();
    XHomog.block(0,0,3,XC) = X;

    float ev;
    int32_t x, y, i, er,ec;
    double *mdat, *probsdat=probs.data(), *mmdat=mm.data();
    const double* edat;
    for( const Cu::Matrix& edge : EP.first )
    {
        er=edge.rows();
        ec=edge.cols();
        edat = edge.data();
        mul = (*(iP++))*XHomog;
        mdat = mul.data();

        ev = 0.0f;
        for(i=0; i<XC; i++)
        {
            x = static_cast<uint32_t>(std::floor(mdat[3*i+1]/mdat[3*i+2]));
            y = static_cast<uint32_t>(std::floor(mdat[3*i]/mdat[3*i+2]));
            ev = ( x>0 && y>0 && x < er && y < ec) ? edat[er*y+x] : 0;

            probsdat[i*EPN + index] = ev;
            mmdat[index] += ev;
        }
        ++index;
    }

    Cu::Matrix cwiseProd = (mm.colwise().prod());
    Cu::Matrix littlep = probs.colwise().prod() / cwiseProd.data()[0];
    if(cwiseProd.data()[0] > 0)
    {
        Cu::Matrix rwiseMean = littlep.rowwise().mean();
        float thresh = rwiseMean.data()[0];
        return Core::find( littlep, [&](const float a){ return a > thresh; } );
    }
    else
    {
        return std::vector<uint32_t>(0);
    }
}




bool SaveTotal(QString filename, QList<Mesh *> surfaces, bool make_poisson, bool colorize, QString mlp_filename)
{
    std::stringstream folder;
    folder << "/tmp/AssetSketchFiles";
    std::stringstream cmdmkdir;
    cmdmkdir << "mkdir " << folder.str();
    std::system(cmdmkdir.str().c_str());

    std::cout << (make_poisson ? "make poisson" : "no poisson") << std::endl;
    std::cout << (colorize ? "colourize" : "no colour") << std::endl;
    CMesh mesh;
    for(auto m : surfaces)
    {
        vcg::tri::Append<CMesh,CMesh>::Mesh(mesh,m->mesh);
    }

    if(!make_poisson && !colorize)
    {
        vcg::tri::io::ExporterPLY<CMesh>::Save(mesh, filename.toStdString().c_str(), vcg::tri::io::Mask::IOM_VERTNORMAL, false);
        return true;
    }

    std::stringstream sspreedits, sspoisson;
    sspreedits << folder.str() << "/asketchtempnamepreedit.ply";
    sspoisson << folder.str() << "/asketchtempnamepoisson.ply";

    vcg::tri::io::ExporterPLY<CMesh>::Save(mesh, sspreedits.str().c_str(), vcg::tri::io::Mask::IOM_VERTNORMAL, false);
    if( make_poisson )
    {
        std::stringstream sscommand;
        sscommand << "../scripts/ComputePoissonSurface.sh " << sspreedits.str() << " " << sspoisson.str();
        uint32_t es = std::system(sscommand.str().c_str());

        if(es != 0)
        {
            std::cerr << "Error processing ComputePoissonSurface.sh" << std::endl;
            return false;
        }
    }
    else
    {
        std::stringstream sscommand;
        sscommand << "cp " << sspreedits.str() << " " << sspoisson.str();
        std::system(sscommand.str().c_str());
    }

    if( colorize )
    {
        uint32_t octave = std::system("../scripts/CheckOctave.sh");
        if( octave != 0 )
        {
            return false;
        }
        std::stringstream sscommand;
        sscommand << "../scripts/ColourSurface.m " << mlp_filename.toStdString() << " " << folder.str() << " " << sspoisson.str() << " " << filename.toStdString();
        uint32_t oout = std::system(sscommand.str().c_str());
        return oout == 0;
    }
    else
    {
        std::stringstream sscommand;
        sscommand << "cp " << sspoisson.str() << " " << filename.toStdString();
        std::system(sscommand.str().c_str());
        return true;
    }
}
